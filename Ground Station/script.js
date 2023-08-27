const portSelector = document.getElementById('portSelector');
const baudRateSelector = document.getElementById('baudRateSelector');
const connectButton = document.getElementById('connectButton');
const disconnectButton = document.getElementById('disconnectButton');
const configFileInput = document.getElementById('configFile');
const chartCanvas = document.getElementById('chartCanvas');
const chart = new Chart(chartCanvas.getContext('2d'), {
  // ... (chart configuration as in the previous script.js)
});

let selectedPort = null;
let selectedBaudRate = 9600; // Default baud rate

// Function to update the chart
function updateChart(value) {
  // ... (unchanged)
}

// Function to fetch data from Arduino
async function fetchData() {
  if (selectedPort) {
    // Fetch data from Arduino using the selected baud rate
    // ... (communication code here)
  }
  setTimeout(fetchData, 1000); // Fetch data every 1 second
}

// Function to list available USB devices
async function listUSBDevices() {
  // ... (unchanged)
}

// Function to connect to the selected USB port
async function connectUSB() {
  // ... (unchanged)
}

// Function to disconnect/reset USB
function disconnectUSB() {
  // ... (disconnect/reset code here)
}

// Function to handle config file upload
configFileInput.addEventListener('change', event => {
  const file = event.target.files[0];
  if (file) {
    // Read and process the config file
    const reader = new FileReader();
    reader.onload = function(event) {
      const configData = JSON.parse(event.target.result);
      // Process the config data as needed
      // ... (config processing code here)
    };
    reader.readAsText(file);
  }
});

// Populate USB port selector
listUSBDevices();

// Event listeners
connectButton.addEventListener('click', connectUSB);
disconnectButton.addEventListener('click', disconnectUSB);
