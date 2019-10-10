from flask import Flask, request

app = Flask(__name__)

f = open("out", "a+")

@app.route("/", methods = ["POST"])
def default():
    f.write(request.data.decode("utf-8"))
    f.flush()
    val = [ int(x) for x in request.data.decode("utf-8").split("\n") if len(x) > 0 ]


if __name__ == "__main__":
    app.run("0.0.0.0", port=9999)
