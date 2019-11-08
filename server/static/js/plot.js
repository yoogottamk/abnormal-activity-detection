window.plot = function (inData, outData) {
    const ctx = document.getElementById('myChart').getContext('2d');
    const labels = [];
    for (let i = 1, len = inData.length; i <= len; i++) {
        labels.push(i);
    }

    console.log(inData);
    console.log(outData);
    var myChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels,
            datasets: [{
                label: 'ESP data',
                data: [inData],
                borderWidth: 1
            }]
        },
        options: {
            responsive: true,
            title: {
                display: true,
                text: 'ESP data'
            },
            tooltips: {
                mode: 'index',
                intersect: false,
            },
            hover: {
                mode: 'nearest',
                intersect: true
            },
            scales: {
                xAxes: [{
                    display: true,
                    scaleLabel: {
                        display: true,
                        labelString: 'Seconds'
                    }
                }],
                yAxes: [{
                    display: true,
                    scaleLabel: {
                        display: true,
                        labelString: 'Value (volts)'
                    }
                }]
            }
        }
    });
};