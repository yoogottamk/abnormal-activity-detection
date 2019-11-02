from flask import Flask, request
from invoker import start, read, write, terminate
import time

app = Flask(__name__)

f = open("out", "a+")

@app.route("/", methods=["POST", "GET"])
def evaluate_data():
    response = request.data.decode("utf-8")

    f.write(request.data.decode("utf-8"))
    f.flush()

    process = start("./checker")
    response, err = process.communicate(response + "-1\n".encode())
    terminate(process)

    response = response.decode("utf-8")

    return "1" if response.find("1") != -1 else "0"

@app.route("/test/", methods=["POST", "GET"])
def test():
    """
    Only for testing
    """

    return "Test"

if __name__ == "__main__":
    app.run("0.0.0.0", port=9999, debug=True, use_reloader=True)
