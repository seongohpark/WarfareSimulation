import { useEffect, useRef, useState } from "react";

// 중계 서버는 브라우저를 실행하는 PC에서 실행합니다.
const BRIDGE_URL = "ws://127.0.0.1:8081";

function App() {
  const [ipAddress, setIpAddress] = useState("");
  const [port, setPort] = useState("");
  const [connectionState, setConnectionState] = useState("idle");
  const [connectionMessage, setConnectionMessage] = useState("미접속");
  const socketRef = useRef(null);
  const timerRef = useRef(null);

  useEffect(() => () => {
    clearTimeout(timerRef.current);
    const socket = socketRef.current;
    socketRef.current = null;
    socket?.close();
  }, []);

  function handleConnect() {
    if (socketRef.current) return;
    const ip = ipAddress.trim();
    const octets = ip.split(".");
    if (octets.length !== 4 || !octets.every(
      (part) => /^(0|[1-9]\d{0,2})$/.test(part) && Number(part) <= 255
    )) {
      setConnectionMessage("올바른 IPv4 주소를 입력하세요.");
      return;
    }
    if (!/^\d+$/.test(port) || Number(port) < 1 || Number(port) > 65535) {
      setConnectionMessage("PORT는 1~65535 정수로 입력하세요.");
      return;
    }

    setConnectionState("connecting");
    setConnectionMessage(`${ip}:${port} 접속 중…`);
    let socket;
    let hasError = false;
    try {
      socket = new WebSocket(BRIDGE_URL);
    } catch (error) {
      setConnectionState("idle");
      setConnectionMessage(`중계 서버 연결 오류: ${error.message}`);
      return;
    }
    socketRef.current = socket;
    timerRef.current = setTimeout(() => {
      if (socketRef.current !== socket) return;
      hasError = true;
      setConnectionMessage("연결 시간이 초과되었습니다. 중계 서버와 TCP 서버를 확인하세요.");
      socket.close();
    }, 10000);

    socket.onopen = () => {
      if (socketRef.current !== socket) return;
      socket.send(JSON.stringify({ type: "connect", ip, port: Number(port) }));
    };
    socket.onmessage = (event) => {
      if (socketRef.current !== socket) return;
      let message;
      try { message = JSON.parse(event.data); }
      catch { return; }
      if (message.type === "connected") {
        clearTimeout(timerRef.current);
        setConnectionState("connected");
        setConnectionMessage(`TCP 접속 완료: ${message.ip}:${message.port}`);
      } else if (message.type === "error" || message.type === "disconnected") {
        hasError = true;
        clearTimeout(timerRef.current);
        setConnectionMessage(message.message);
        socket.close();
      } else if (message.type === "data") {
        console.log("TCP 수신 데이터(Base64):", message.base64);
      }
    };
    socket.onerror = () => {
      if (socketRef.current !== socket) return;
      hasError = true;
      setConnectionMessage("중계 서버 연결 실패: tcp_bridge.py 실행 여부를 확인하세요.");
    };
    socket.onclose = () => {
      if (socketRef.current !== socket) return;
      clearTimeout(timerRef.current);
      socketRef.current = null;
      setConnectionState("idle");
      if (!hasError) setConnectionMessage("연결이 종료되었습니다.");
    };
  }

  const gameTime = "2026-10-16 14:30:00";
  const serverTime = "2026-10-16 14:30:00";

  return (
    <div style={{ padding: "24px" }}>
      <fieldset
        style={{
          margin: 0,
          padding: "20px",
          border: "1px solid #9ca3af",
          borderRadius: "8px",
        }}
      >
        <legend style={{ padding: "0 8px", fontWeight: "bold" }}>
          시뮬레이션 통제
        </legend>

        <div
          style={{
            display: "grid",
            gridTemplateColumns: "max-content max-content",
            columnGap: "12px",
            rowGap: "8px",
            marginBottom: "20px",
            fontSize: "16px",
          }}
        >
          <span>게임시간 :</span>
          <time dateTime="2026-10-16T14:30:00">{gameTime}</time>

          <span>서버시간 :</span>
          <time dateTime="2026-10-16T14:30:00">{serverTime}</time>
        </div>

        <div style={{ display: "flex", gap: "12px", flexWrap: "wrap" }}>
          {["START", "PAUSE", "RESUME", "STOP"].map((command) => (
            <button
              key={command}
              type="button"
              onClick={() => alert(`${command} 클릭`)}
              style={{
                padding: "10px 20px",
                fontSize: "16px",
                cursor: "pointer",
              }}
            >
              {command}
            </button>
          ))}
        </div>
      </fieldset>

      <fieldset
        style={{
          margin: "24px 0 0",
          padding: "20px",
          border: "1px solid #9ca3af",
          borderRadius: "8px",
        }}
      >
        <legend style={{ padding: "0 8px", fontWeight: "bold" }}>
          서버 정보
        </legend>

        <div
          style={{
            display: "grid",
            gridTemplateColumns: "max-content minmax(0, 240px) max-content",
            alignItems: "center",
            columnGap: "12px",
            rowGap: "12px",
            fontSize: "16px",
          }}
        >
          <label htmlFor="server-ip-address">IP ADDRESS</label>
          <input
            id="server-ip-address"
            name="serverIpAddress"
            value={ipAddress}
            onChange={(event) => setIpAddress(event.target.value)}
            disabled={connectionState !== "idle"}
            type="text"
            placeholder="예: 192.168.2.107"
            autoComplete="off"
            spellCheck={false}
            style={{ boxSizing: "border-box", width: "100%", minWidth: 0, padding: "8px", fontSize: "16px" }}
          />

          <button
            type="button"
            onClick={handleConnect}
            disabled={connectionState !== "idle"}
            style={{
              gridColumn: "3",
              gridRow: "1",
              padding: "10px 20px",
              fontSize: "16px",
              cursor: "pointer",
            }}
          >
            {connectionState === "connecting" ? "접속 중…" : connectionState === "connected" ? "접속됨" : "접속"}
          </button>

          <label htmlFor="server-port" style={{ gridColumn: "1" }}>PORT</label>
          <input
            id="server-port"
            name="serverPort"
            value={port}
            onChange={(event) => setPort(event.target.value)}
            disabled={connectionState !== "idle"}
            type="number"
            min="1"
            max="65535"
            step="1"
            placeholder="예: 8080"
            style={{ boxSizing: "border-box", width: "100%", minWidth: 0, padding: "8px", fontSize: "16px" }}
          />
        </div>

        <p role="status" style={{ marginBottom: 0 }}>{connectionMessage}</p>
      </fieldset>
    </div>
  );
}

export default App;
