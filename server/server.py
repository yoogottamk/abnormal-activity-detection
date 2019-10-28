from flask import Flask, request
from invoker import start, read, write
import time

app = Flask(__name__)

f = open("out", "a+")

process = start("./checker")

@app.route("/", methods=["POST", "GET"])
def boomboom():
    # response = request.data.decode("utf-8")
    response = request.args.get("data")

    # f.write(request.data.decode("utf-8"))
    # f.flush()

    write(process, request.data.decode("utf-8"))
    write(process, "-1")
    response = read(process)

    print("GOT", response)

    return "1" if response.find("1") != -1 else "0"

@app.route("/test/", methods=["POST", "GET"])
def test():
    """
    Only for testing
    """

    return "Test"

if __name__ == "__main__":
    app.run("0.0.0.0", port=9999, debug=True, use_reloader=True)
