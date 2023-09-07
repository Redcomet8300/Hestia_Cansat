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

    let eventSource;

    document.getElementById('connectButton').addEventListener('click', () => {
        const selectedPort = document.getElementById('portSelect').value;
        const selectedBaudRate = document.getElementById('baudRateSelect').value;

        if (!eventSource) {
            fetch('/connect', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded',
                },
                body: `port=${selectedPort}&baudRate=${selectedBaudRate}`,
            }).then(response => {
                if (response.ok) {
                    eventSource = new EventSource('/data');
                    chart.data.labels = [];
                    chart.data.datasets[0].data = [];
                    chart.update();
                    eventSource.onmessage = function (event) {
                        const data = JSON.parse(event.data);
                        chart.data.labels.push('');
                        chart.data.datasets[0].data.push(data);
                        chart.update();
                    };
                } else {
                    alert('Connection failed. Please check the port and baud rate.');
                }
            });
        }
    });

    document.getElementById('disconnectButton').addEventListener('click', () => {
        if (eventSource) {
            eventSource.close();
            eventSource = null;
            fetch('/disconnect', { method: 'POST' });
        }
    });
});
