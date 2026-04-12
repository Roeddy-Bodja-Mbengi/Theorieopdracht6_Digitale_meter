using System;
using System.IO.Ports;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Digitale_Meter
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        SerialPort serialPort;
        Action<string> verwerkData;
        string buffer = "";
        Regex verbruikRegex = new Regex(@"1-0:1\.7\.0\((.*?)\*kW\)");
        Regex injectieRegex = new Regex(@"1-0:2\.7\.0\((.*?)\*kW\)");

        public MainWindow()
        {
            InitializeComponent();
            
            string[] poorten = SerialPort.GetPortNames();
            foreach (string poort in poorten)
            {
                cmbxPoorten.Items.Add(poort);
            }

            verwerkData = VerwerkP1Data;
        }
        void VerwerkP1Data(string data)
        {
            Dispatcher.Invoke(() =>
            {
                if (data.Contains("!")) 
                { 
                     LedBranden();

                }

                Match verbruikMatch = verbruikRegex.Match(data);
                Match injectieMatch = injectieRegex.Match(data);

                if (verbruikMatch.Success)
                {
                    string verbruik = verbruikMatch.Groups[1].Value;
                    txtVermogen.Text = verbruik + "kW";
                    txtVermogen.Background = Brushes.Red;
                }
                else if(injectieMatch.Success)  
                {
                    string injectie = injectieMatch.Groups[1].Value;
                    txtVermogen.Text = injectie + "kW";
                    txtVermogen.Background = Brushes.Green;
                }

            });
        }
        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string data = serialPort.ReadExisting();
            buffer += data;

            while (buffer.Contains("!"))
            {
                int index = buffer.IndexOf("!");
                string telegram = buffer.Substring(0, index + 1);

                verwerkData(telegram);

                buffer = buffer.Substring(index + 1);
            }
        }

        private void cmbxPoorten_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
        

            if (cmbxPoorten.SelectedItem != null)
            {

                if (serialPort != null && serialPort.IsOpen)
                {
                    serialPort.Close();
                }


                string geselecteerdePoort = cmbxPoorten.SelectedItem.ToString();

                serialPort = new SerialPort(geselecteerdePoort, 115200);
                serialPort.DataReceived += SerialPort_DataReceived;
                serialPort.Open();

                if (serialPort.IsOpen)
                {
                    cmbxPoorten.IsEnabled = false;
                }

            }


        }

        async void LedBranden()
        {
            kleineLed.Fill = Brushes.Blue;
            await Task.Delay(100);
            kleineLed.Fill = Brushes.Gray;
        }

        protected override void OnClosed(EventArgs e)
        {
            if (serialPort != null && serialPort.IsOpen)
            {
                serialPort.Close();
            }

            base.OnClosed(e);
        }
    }
}