from flask import Flask, request, render_template, jsonify
from datetime import datetime
import threading
import time
import os
import requests
import json
import re

from invoker import start, read, write, terminate
from bot import send_text, send_graph


def append_to_file(fname, data):
    with open(fname, "a+") as f:
        f.write(data)


def write_to_file(fname, data):
    with open(fname, "w+") as f:
        f.write(data)


app = Flask(__name__, static_folder="static")

ESP_DOWN_FILENAME = "../ESP_DOWN"
SERVER_DOWN_FILENAME = "../SERVER_DOWN"
BUZZ_NEXT = False

lastSentToOneM2M = time.time()
RATE_LIMIT_OM2M = 60


def currentTimestampFormatted():
    now = datetime.now()
    # dd/mm/YY H:M:S
    dt_string = now.strftime("%d/%m/%Y %H:%M:%S")
    return dt_string

# send val to IIIT onem2m server


def sendOneM2Mrequest(val):
    global lastSentToOneM2M
    if int(time.time()) - lastSentToOneM2M < RATE_LIMIT_OM2M:
        return

    cse_ip = "onem2m.iiit.ac.in"
    cse_port = "443"
    server = "http://"+cse_ip+":"+cse_port+"/~/in-cse/in-name/"
    ae = "Team34_Abnormal_activity_detection_outside_of_classroom"
    cnt = "node_1"

    url = server + ae + "/" + cnt + "/"
    payload = {
        "m2m:cin": {
            "cnf": "text/plain:0",
            "con": val
        }
    }
    headers = {
        "X-M2M-Origin": "admin:admin",
        "Content-Type": "application/json;ty=4",
        "Content-Length": "100",
        "Connection": "close"
    }

    lastSentToOneM2M = int(time.time())

    r = requests.post(url, data=json.dumps(payload), headers=headers)
    # print(lastSentToOneM2M)
    # print(r)


# all input data we have received so far
# and our corresponding output on it
data_so_far = [["1"], ["0"]]
last_updated_data = ""
seconds_to_keep_for = 10
samples_per_sec = 600
data_count_to_retain = samples_per_sec * seconds_to_keep_for
graph_step = seconds_to_keep_for / data_count_to_retain
BUZZ_ENABLED = True
reg = re.compile(r"(\d{3})")
rep = r"\1 "
freg = r" "
frep = r"0\n"
SEP = " "


def extras(response, output, shouldBuzzerBlow):
    global data_so_far
    global last_updated_data

    # decoded input
    data = re.sub(reg, rep, response.decode("utf-8"))

    # hotfix, ignore
    fdata = data[:data.find("-1")]
    fdata = re.sub(freg, frep, fdata.strip() + " ")

    # add latest data and trim to the count we wish to retain
    data_so_far[0] += data.split(" ")
    data_so_far[1] += list(output)
    data_so_far = [data_so_far[0][-data_count_to_retain:],
                   data_so_far[1][-data_count_to_retain:]]
    last_updated_data = currentTimestampFormatted()

    append_to_file("out", fdata)
    write_to_file("live", str(time.time()).split('.')[0])

    if output.find("1") != -1:
        send_graph(data, output, "Found anomaly")

    sendOneM2Mrequest(shouldBuzzerBlow)


@app.route("/", methods=["POST", "GET"])
def evaluate_data():
    global BUZZ_NEXT

    if os.path.exists(ESP_DOWN_FILENAME):
        os.remove(ESP_DOWN_FILENAME)
        send_text("ESP back up!")

    if os.path.exists(SERVER_DOWN_FILENAME):
        os.remove(SERVER_DOWN_FILENAME)
        send_text("Flask server is up!")

    response = request.data

    if BUZZ_NEXT:
        BUZZ_NEXT = False
        return "1"

    process = start("./checker")
    output, err = process.communicate(response + "-1\n".encode())
    if err:
        print(err)
    terminate(process)
    # decoded output
    output = output.decode("utf-8")

    shouldBuzzerBlow = "1" if BUZZ_ENABLED and output.find("1") != -1 else "0"

    threading.Thread(target=extras, args=(
        response, output, shouldBuzzerBlow)).start()

    return shouldBuzzerBlow


@app.route("/get-data/", methods=["GET"])
def get_data():
    global data_so_far

    # how to return a dict here?
    return jsonify({"input": " ".join(data_so_far[0]), "output": " ".join(data_so_far[1])})


@app.route("/home/", methods=["GET"])
def render_home():
    global data_so_far
    auto_reload = True

    if request.args.get("auto_reload") == "False":
        auto_reload = False

    return render_template("home.html", input=" ".join(data_so_far[0]), output=" ".join(data_so_far[1]), auto_reload=auto_reload, step=graph_step, timestamp=last_updated_data)


@app.route("/enable/", methods=["GET", "POST"])
def enableBuzz():
    global BUZZ_ENABLED
    BUZZ_ENABLED = True
    return "1"


@app.route("/disable/", methods=["GET", "POST"])
def disableBuzz():
    global BUZZ_ENABLED
    BUZZ_ENABLED = False
    return "1"


@app.route("/status/", methods=["GET", "POST"])
def buzzStatus():
    return "1" if BUZZ_ENABLED else "0"


@app.route("/BUZZ/", methods=["GET", "POST"])
def override_buzzer():
    global BUZZ_NEXT
    BUZZ_NEXT = True
    return "Done!"


@app.route("/test/", methods=["GET", "POST"])
def testAliveURL():
    return "1"


if __name__ == "__main__":
    app.run("0.0.0.0", port=9999, debug=True, use_reloader=True)
