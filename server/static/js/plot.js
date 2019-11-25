function round(x) {
    let scale = 100000;
    return Math.round(x * scale) / scale;
}

const normal = "rgba(74,192,192,1)", red = "rgba(200,50,50,1)";
window.plot = function (inData, outData, step) {
    const canvas = document.getElementById("myChart");
    const ctx = canvas.getContext('2d');
    const labels = [];
    step = round(step);
let c = 0;

    for (let i = 1, len = inData.length; i <= len; i++) {
        labels.push(round(c));
        c += step;
    }

    if (parseInt(inData[0]) < 50) {
        inData.shift();
        labels.pop();
    }

    let shouldColorRed = outData.indexOf(1) != -1;

    ctx.clearRect(0, 0, canvas.width, canvas.height);
    new Chart(ctx).Line({
        labels,
        datasets: [{
            label: "Sound intensity for last ten seconds",
            /* TODO make it red too! */
            borderColor: shouldColorRed ? red : normal,
            /*
        fillColor: "rgba(220,220,220,0.2)",
        strokeColor: "rgba(220,220,220,1)",
        pointColor: "rgba(220,220,220,1)",
        pointStrokeColor: "#fff",
        pointHighlightFill: "#fff",
        pointHighlightStroke: "rgba(220,220,220,1)",
        */
            fill: false,
            data: inData
        }]
    }/*,
        {
        responsive: true,
            /*
        scaleLineColor: "rgba(0,0,0,.2)",
        segmentShowStroke: false,


        animation: {
        duration: 0,
        },
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
    }*/
        );
};

const REFRESH_INTERVAL = 500;

function query(step) {
    var oReq = new XMLHttpRequest();
    oReq.addEventListener("load", (resp) => {
        resp = JSON.parse(resp.target.response);
        processData(resp.input, resp.output, step, true);
    });
    oReq.open("GET", "/get-data/");
    oReq.send();
}

window.processData = function (inString, outString, step, AUTO_RELOAD_ENABLED) {
    const inData = inString.split(" ").map(x => parseInt(x) * 10 + Math.floor(Math.random() * 10));
    const outData = outString.split(" ").map(x => parseInt(x));
    console.log("plotting", inData, outData);

    window.plot(inData, outData, step);

    if (AUTO_RELOAD_ENABLED) {
        setTimeout(query, REFRESH_INTERVAL, step);
    }
};
