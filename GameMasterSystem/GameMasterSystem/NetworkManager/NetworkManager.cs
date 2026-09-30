using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace GameMasterSystem
{
	public class NetworkManager
	{
		private Socket? clientSocket;
        private CancellationTokenSource? receiveCts;

        public async Task<bool> ConnectAsync(string serverIp, int serverPort)
        {
            try
            {
                IPAddress ipAddress = IPAddress.Parse(serverIp);
                IPEndPoint endPoint = new IPEndPoint(ipAddress, serverPort);

                clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                await clientSocket.ConnectAsync(endPoint);

                receiveCts = new CancellationTokenSource();
                _ = ReceiveLoopAsync(receiveCts.Token);

                return true;
            }
            catch (SocketException ex)
            {
                Console.WriteLine($"Connect Error : {ex.Message}");
                return false;
            }
        }

        private async Task ReceiveLoopAsync(CancellationToken token)
        {
            if (clientSocket == null)
            {
                return;
            }   

            byte[] buffer = new byte[4096];

            try {
                while (!token.IsCancellationRequested)
                {
                    int receivedBytes = await clientSocket.ReceiveAsync(buffer, SocketFlags.None, token);
                    if (receivedBytes == 0)
                    {
                        Console.WriteLine("Server disconnected.");
                        break;
                    }

                    string message = Encoding.UTF8.GetString(buffer, 0, receivedBytes);
                    Console.WriteLine($"Receive : {message}");
                }
            }
            catch (OperationCanceledException)
            {
                // 정상적인 수신 루프 종료
            }
            catch (SocketException ex)
            {
                Console.WriteLine($"Receive Error : {ex.Message}");
            }
        }

        public bool Connect(string serverIp, int serverPort)
		{
			try{
				IPAddress ipAddress = IPAddress.Parse(serverIp);
                IPEndPoint serverEndPoint = new IPEndPoint(ipAddress, serverPort);

                clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                clientSocket.Connect(serverEndPoint);

            }
            catch (SocketException ex)
            {                
                return false;
            }
            catch (Exception ex)
            {                
                return false;
            }

			return true;
		}

        public string? Receive()
        {
            if (clientSocket == null || !clientSocket.Connected)
            {
                return null;
            }                

            try{
                byte[] buffer = new byte[4096];
                int receivedBytes = clientSocket.Receive(buffer);

                return Encoding.UTF8.GetString(buffer, 0, receivedBytes);
            }
            catch (SocketException ex)
            {                
                return null;
            }
        }

    }
}