from flask import Flask, request
import numpy as np

app = Flask(__name__)

@app.route("/", methods = ["GET", "POST"])
def default():
    val = [ int(x) for x in request.data.decode("utf-8")[:-1].split(",") ]
    return "TEST"
    return str(np.average(np.array(val, np.uint8)))

if __name__ == "__main__":
    app.run("0.0.0.0", port=9999)
