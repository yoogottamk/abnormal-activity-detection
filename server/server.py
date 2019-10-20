from flask import Flask, request
from invoker import start, read, write

app = Flask(__name__)

f = open("out", "a+")

process = start("./checker")

@app.route("/", methods = ["POST"])
def default():
    f.write(request.data.decode("utf-8"))
    f.flush()

    write(process, request.data.decode("utf-8"))
    response = read(process)
    write(process, "-1")


    x = "1" if response.find("1") != -1 else "0"
    print("Sending ", x)
    return x

if __name__ == "__main__":
    app.run("0.0.0.0", port=9999)
