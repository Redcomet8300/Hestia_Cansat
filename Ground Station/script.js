const portSelector = document.getElementById('portSelector');
const baudRateInput = document.getElementById('baudRateInput'); // Changed from baudRateSelector
const connectButton = document.getElementById('connectButton');
const disconnectButton = document.getElementById('disconnectButton');
const chartCanvas = document.getElementById('chartCanvas');
const dataTable = document.getElementById('dataTable'); // Removed unnecessary {}

const tableBody = dataTable.querySelector('tbody');
const chart = new Chart(chartCanvas.getContext('2d'), {
  // ... (chart configuration as in the previous script.js)
});

// Function to update the chart and table with new data
function updateChartAndTable(time, value) {
  // Update the chart
  chart.data.labels.push(time);
  chart.data.datasets[0].data.push(value);
  chart.update();

  // Update the table
  const newRow = tableBody.insertRow();
  newRow.insertCell(0).textContent = time;
  newRow.insertCell(1).textContent = value;
}

// Function to simulate receiving data from Arduino
function receiveDataFromArduino(time, value) {
  updateChartAndTable(time, value);
}

let selectedPort = null;

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

// Event listeners
connectButton.addEventListener('click', connectUSB);
disconnectButton.addEventListener('click', disconnectUSB);