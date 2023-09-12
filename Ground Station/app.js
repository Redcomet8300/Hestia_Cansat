document.addEventListener('DOMContentLoaded', function () {
    const ctx = document.getElementById('myChart').getContext('2d');
    const chart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: [],
            datasets: [{
                label: 'Sensor Data',
                data: [],
                borderColor: 'blue',
                borderWidth: 1,
            }]
        },
        options: {
            scales: {
                x: {
                    type: 'linear',
                    position: 'bottom',
                },
            },
        }
    });

    let socket;

    document.getElementById('connectButton').addEventListener('click', () => {
        const selectedPort = document.getElementById('portSelect').value;
        const selectedBaudRate = document.getElementById('baudRateInput').value;
        
        // Create a WebSocket connection
        socket = new WebSocket(`ws://localhost:8080/ws`);

        socket.onopen = () => {
            console.log('WebSocket opened');
        };

        socket.onmessage = (event) => {
            const data = JSON.parse(event.data);
            const timestamp = data.timestamp;
            const sensorValue = data.sensorValue;

            chart.data.labels.push(timestamp);

            chart.data.datasets[0].data.push(sensorValue);

            const maxDataPoints = 20;
            if (chart.data.labels.length > maxDataPoints) {
                chart.data.labels.shift();
                chart.data.datasets[0].data.shift();
            }

            // Update the chart
            chart.update();
        };

        socket.onclose = () => {
            console.log('WebSocket closed');
        };

        socket.onerror = (error) => {
            console.error(`WebSocket error: ${error.message}`);
        };
    });

    document.getElementById('disconnectButton').addEventListener('click', () => {
        if (socket) {
            socket.close();
            socket = null;
        }
    });
});
