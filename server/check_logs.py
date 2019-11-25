import sys
import re

from invoker import start, terminate

for logFile in sys.argv[1:]:
    with open(logFile, "r") as f:
        text = f.read()

    vals = list(map(lambda x: ('0'*(4 - len(x)) + x)[:-1], text.split("\n")[:-1]))

    proc = start("./checker")
    output, err = proc.communicate((''.join(vals) + " -1\n").encode())
    terminate(proc)

    print(output)
