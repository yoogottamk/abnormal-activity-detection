from flask import Flask, request
import threading
import time
import os

from invoker import start, read, write, terminate
from bot import send_text, send_graph

def append_to_file(fname, data):
    with open(fname, "a+") as f:
        f.write(data)

def write_to_file(fname, data):
    with open(fname, "w+") as f:
        f.write(data)

app = Flask(__name__)

ESP_DOWN_FILENAME = "./ESP_DOWN"

# routes
@app.route("/", methods=["POST", "GET"])
def evaluate_data():
    if os.path.exists(ESP_DOWN_FILENAME):
        os.remove("./ESP_DOWN")
        send_text("ESP back up!")

    response = request.data

    process = start("./checker")
    output, err = process.communicate(response + "-1\n".encode())
    terminate(process)

    # decoded input
    data = response.decode("utf-8")
    #decoded output
    output = output.decode("utf-8")

    append_to_file("out", data)
    write_to_file("live", str(time.time()).split('.')[0])

    if output.find("1") != -1:
        threading.Thread(target=send_graph, args=(data,output,"Found anomaly")).start()

    return "1" if output.find("1") != -1 else "0"

@app.route("/test/", methods=["POST", "GET"])
def test():
    """
    Only for testing
    """

    return "Test"

if __name__ == "__main__":
    app.run("0.0.0.0", port=9999, debug=True, use_reloader=True)
