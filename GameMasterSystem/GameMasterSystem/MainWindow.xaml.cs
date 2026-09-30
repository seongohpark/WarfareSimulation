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
            const string serverIp = "127.0.0.1";
            const int serverPort = 3001;

            await networkManager.ConnectAsync(serverIp, serverPort);
        }
    }    
}
