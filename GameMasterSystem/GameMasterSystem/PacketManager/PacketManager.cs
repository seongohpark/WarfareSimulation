using System;
using System.Text;

namespace GameMasterSystem
{
    public class PacketManager
    {
        public enum ControlPacketType
        {
            START = 0,
            PAUSE = 1,
            RESUME = 2,
            STOP = 3
        }

        public PacketManager()
        {
        }

        public string CreateControlPacket(ControlPacketType packetType)
        {
            string packet = "";

            switch (packetType)
            {
            case ControlPacketType.START:
                {
                    string header = "$HEAD";
                    string sendId = "S10";
                    string recvId = "R16";

                    string currentTime = DateTime.Now.ToString("yyyyMMddHHmmss");
                    string date = "D" + currentTime;
                    string processId = "P1600010";
                    string data = $"<{packetType}|{currentTime}>";
                    string tail = "TAIL@";

                    packet = header + "00000" + sendId + recvId + date + processId + data + tail;
                    int packetLength = Encoding.ASCII.GetByteCount(packet);
                    string length = packetLength.ToString("D5");

                    packet = header + length + sendId + recvId + date + processId + data + tail;
                }
                break;

            case ControlPacketType.PAUSE:
                {

                }
            break;
            
            case ControlPacketType.RESUME:
                {

                }
                break;

            case ControlPacketType.STOP:
                {

                }
            break;

            default:
                {

                }
            break;
            }

            return packet;
        }
    }
}