const express = require('express');
const app = express();
const port = 3000;

// Serve the HTML file
app.get('/', (req, res) => {
  res.sendFile(__dirname + '/index.html');
});

// Simulate getting sensor data
app.get('/getSensorData', (req, res) => {
  // In a real scenario, read data from a database or hardware
  const sensorValue = Math.random() * 100;
  res.send(sensorValue.toString());
});

// Simulate toggling LED
app.post('/toggleLED', (req, res) => {
  // In a real scenario, send command to the Arduino
  console.log('LED toggled');
  res.sendStatus(200);
});

app.listen(port, () => {
  console.log(`Server listening at http://localhost:${port}`);
});
