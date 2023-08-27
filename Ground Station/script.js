const portSelector = document.getElementById('portSelector');
const connectButton = document.getElementById('connectButton');
const chartCanvas = document.getElementById('chartCanvas');
const chart = new Chart(chartCanvas.getContext('2d'), {
  // ... (chart configuration as in the previous script.js)
});

// Function to update the chart
function updateChart(value) {
  chart.data.labels.push(dataCounter++);
  chart.data.datasets[0].data.push(value);
  chart.update();
}

// Function to fetch data from Arduino
async function fetchData() {
  // Simulated data fetching as before
}

// Function to list available USB devices
async function listUSBDevices() {
  const devices = await navigator.usb.getDevices();
  devices.forEach(device => {
    portSelector.innerHTML += `<option value="${device.productName}">${device.productName}</option>`;
  });
}

// Function to connect to the selected USB port
async function connectUSB() {
  const selectedPort = portSelector.value;
  const devices = await navigator.usb.getDevices();
  const selectedDevice = devices.find(device => device.productName === selectedPort);

  if (selectedDevice) {
    // Use selectedDevice for communication
    // Note: Actual WebUSB connection code goes here
    // Refer to WebUSB documentation for implementation details

    // Fetch data and update chart
    fetchData();
  } else {
    console.error('Selected device not found.');
  }
}

// Populate USB port selector
listUSBDevices();

// Event listeners
connectButton.addEventListener('click', connectUSB);
