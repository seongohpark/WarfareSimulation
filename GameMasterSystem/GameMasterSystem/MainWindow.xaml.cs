using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace GameMasterSystem
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private readonly NetworkManager networkManager;

        public MainWindow()
        {
            InitializeComponent();
            networkManager = new NetworkManager();
        }

        public void GameStartButton_Click(object sender, RoutedEventArgs e)
        {
            Debug.WriteLine("Game Start Button Click");
        }

        public void GameEndButton_Click(object sender, RoutedEventArgs e)
        {
            Debug.WriteLine("Game End Button Click");
        }

        private async void ConnectServerButton_Click(object sender, RoutedEventArgs e)
        {
            string serverAddress = TextBoxServerAddr.Text.Trim();
            string[] parts = serverAddress.Split(':');

            if (parts.Length != 2)
            {
                MessageBox.Show("서버 주소를 IP:PORT 형식으로 입력해주세요.");
                return;
            }

            string serverIp = parts[0];
            if (!int.TryParse(parts[1], out int serverPort))
            {
                MessageBox.Show("Port 번호가 올바르지 않습니다.");
                return;
            }

            Debug.WriteLine($"Server IP   : {serverIp}");
            Debug.WriteLine($"Server Port : {serverPort}");

            await networkManager.ConnectAsync(serverIp, serverPort);
        }
    }    
}
