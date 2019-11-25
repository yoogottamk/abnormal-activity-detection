import sys
import re
import datetime as dt
import os
import numpy as np

from invoker import start, terminate

VALUES_PER_SECOND = 1200

def getCheckerOutputFromLog(logFile):
    if not os.path.exists(logFile):
        return ""

    with open(logFile, "r") as f:
        text = f.read()

    vals = list(map(lambda x: ('0'*(4 - len(x)) + x)[:-1], text.split("\n")[:-1]))

    proc = start("./checker")
    output, err = proc.communicate((''.join(vals) + " -1\n").encode())
    terminate(proc)

    out = np.array(list(output.decode("utf-8")))

    splits = np.array_split(out, VALUES_PER_SECOND)

    output = []

    for split in splits:
        output.append('1' if '1' in split else '0')

    return ''.join(output)


def getHourLog(year,month,date,hour):
    return getCheckerOutputFromLog(f"../data/${year}-${month}-${date}_${hour}.log")

def getDayLog(year,month,date):
    pass
