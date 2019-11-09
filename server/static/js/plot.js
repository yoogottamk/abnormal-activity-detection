window.plot = function (inData, outData) {
    const ctx = document.getElementById('myChart').getContext('2d');
    const labels = [];

    for (let i = 1, len = inData.length; i <= len; i++) {
        labels.push(i);
    }

	if(parseInt(inData[0]) < 50) 
	{
		inData.shift();
		labels.pop();
	}

	
    new Chart(ctx, {
        type: 'line',
        data: {
            labels,
            datasets: [{
		borderColor: "rgba(74,192,192,1)",
//		backgroundColor: "rgba(255,0,0,0)",
                fill: false,
                label: 'ESP data',
                data: inData
            }]
        },
        options: {
            animation: {
                duration: 0,
            },
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
                    ticks: {
                        max: 4095,
                        min: 0
                    },
                    scaleLabel: {
                        display: true,
                        labelString: 'Value (volts)'
                    }
                }]
            }
        }
    });
};
