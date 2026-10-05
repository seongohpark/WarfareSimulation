using System;

namespace GameMasterSystem
{
    public class CommonUtility
    {
        public static string GetCurrentTime()
        {
            string currTime = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
            return currTime;
        }
    }
}