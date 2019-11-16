function round(x){
	let scale = 100000;
	return Math.round(x * scale) / scale;
}

const normal = "rgba(74,192,192,1)", red = "rgba(200,50,50,1)";
window.plot = function (inData, outData, step) {
    const ctx = document.getElementById('myChart').getContext('2d');
    const labels = [];
	step = round(step);
	let c = 0;
	
	for (let i = 1, len = inData.length; i <= len; i++) {
        	labels.push(round(c));
		c += step;
	    }	

	if(parseInt(inData[0]) < 50) 
	{
		inData.shift();
		labels.pop();
	}

	let shouldColorRed = outData.indexOf(1) != -1;
	
    new Chart(ctx, {
        type: 'line',
        data: {
            labels,
            datasets: [{
		    borderColor: shouldColorRed ? red : normal,
//		backgroundColor: "rgba(255,0,0,0)",
                fill: false,
                label: 'ESP data for last ten seconds',
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
