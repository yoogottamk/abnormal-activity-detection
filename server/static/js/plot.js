const normal = "rgba(74,192,192,1)", red = "rgba(200,50,50,1)";
const canvas = document.getElementById("myChart");
const ctx = canvas.getContext('2d');
window.plot = function (inData, outData) {
	let labels = [];
	let c = 0;

	inData = inData.filter(x => parseInt(x) >= 00);

	for (let i = 1, len = inData.length; i <= len; i++) {
		labels.push(i);
	}

	const stepper = Math.round(inData.length / duration / 2);

	let shouldColorRed = outData.indexOf(1) != -1;

	ctx.clearRect(0, 0, canvas.width, canvas.height);
	new Chart(ctx, {
		type: "line",
		data: {
			labels,
			datasets: [{
				label: "Sound intensity for last ten seconds",
				borderColor: shouldColorRed ? red : normal,
				pointColor: shouldColorRed ? red : normal,
				backgroundColor : "rgba(48, 164, 255, 0.2)",
				pointBackgroundColor : "rgba(48, 164, 255, 1)",
				pointRadius: 0.1,
				pointBorderColor: normal,

				pointBorderColor : "#fff",
				data: inData
			}]
		},
		options: {
			responsive: true,
			maintainAspectRatio: false,
			scaleLineColor: "rgba(0,0,0,.2)",
			scaleGridLineColor: "rgba(0,0,0,.05)",
			scaleFontColor: "#c5c7cc",
			animation: {
				duration: 0,
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
					ticks: {
						userCallback: function(item, index) {
							if (!(index % (stepper))) return index / (stepper * 2);
						},
						autoSkip: false
					},
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

const REFRESH_INTERVAL = 1000;

function query() {
	var oReq = new XMLHttpRequest();
	oReq.addEventListener("load", (resp) => {
		resp = JSON.parse(resp.target.response);
		processData(resp.input, resp.output, true);
	});
	oReq.open("GET", "/get-data/");
	oReq.send();
}
let graphplotcount=0;

window.processData = function (inString, outString, AUTO_RELOAD_ENABLED) {
	const inData = inString.split(" ").map(x => parseInt(x) * 10 + Math.floor(Math.random() * 10));
	const outData = outString.split(" ").map(x => parseInt(x));

	if(graphplotcount++ > 50) window.location.reload();

	window.plot(inData, outData);

	if (AUTO_RELOAD_ENABLED) {
		setTimeout(query, REFRESH_INTERVAL);
	}
};
