"""Local WebSocket-to-TCP connection bridge. Python 3.9+ / websockets 14+."""
import asyncio
import base64
import ipaddress
import json

from websockets.asyncio.server import serve
from websockets.exceptions import ConnectionClosed

HOST = "127.0.0.1"
PORT = 8081
ALLOWED_ORIGINS = [
    "http://localhost:5173", "http://127.0.0.1:5173",
    "http://localhost:3000", "http://127.0.0.1:3000",
]


async def handle_client(ws):
    writer = None
    tasks = []
    try:
        raw = await asyncio.wait_for(ws.recv(), timeout=10)
        request = json.loads(raw)
        if not isinstance(request, dict) or request.get("type") != "connect":
            raise ValueError("connect 요청이 필요합니다.")
        ip = str(ipaddress.IPv4Address(request.get("ip", "")))
        port = request.get("port")
        if type(port) is not int or not 1 <= port <= 65535:
            raise ValueError("PORT는 1~65535 정수여야 합니다.")

        reader, writer = await asyncio.wait_for(
            asyncio.open_connection(ip, port), timeout=5
        )
        await ws.send(json.dumps({"type": "connected", "ip": ip, "port": port}))
        print(f"TCP connected: {ip}:{port}", flush=True)

        async def receive_tcp():
            while True:
                data = await reader.read(65536)
                if not data:
                    await ws.send(json.dumps({
                        "type": "disconnected", "message": "TCP 서버가 연결을 종료했습니다."
                    }))
                    return
                # TCP는 스트림입니다. 이 청크 하나가 패킷 하나라는 뜻은 아닙니다.
                await ws.send(json.dumps({
                    "type": "data", "base64": base64.b64encode(data).decode("ascii")
                }))

        tasks = [asyncio.create_task(receive_tcp()),
                 asyncio.create_task(ws.wait_closed())]
        done, _ = await asyncio.wait(tasks, return_when=asyncio.FIRST_COMPLETED)
        for task in done:
            task.result()
    except ConnectionClosed:
        pass
    except (ValueError, TypeError, OSError, asyncio.TimeoutError) as exc:
        message = "TCP 연결 시간이 초과되었습니다." if isinstance(exc, asyncio.TimeoutError) else str(exc)
        try:
            await ws.send(json.dumps({"type": "error", "message": message}))
        except ConnectionClosed:
            pass
    finally:
        for task in tasks:
            if not task.done():
                task.cancel()
        if tasks:
            await asyncio.gather(*tasks, return_exceptions=True)
        if writer is not None:
            writer.close()
            try:
                await writer.wait_closed()
            except OSError:
                pass
        await ws.close()


async def main():
    async with serve(handle_client, HOST, PORT, origins=ALLOWED_ORIGINS,
                     max_size=4096, close_timeout=2):
        print(f"Bridge ready: ws://{HOST}:{PORT}", flush=True)
        await asyncio.Future()


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        pass
