import sys
import re
import datetime as dt
import os
import numpy as np

from invoker import start, terminate

VALUES_PER_ITERATION = 1200

def getCheckerOutputFromLog(logFile):
    if not os.path.exists(logFile):
        print(f"File {logFile} not found")
        return ""

    with open(logFile, "r") as f:
        text = f.read()

    vals = np.array(list(map(lambda x: ('0'*(4 - len(x)) + x)[:-1], text.split("\n")[:-1])))
    splits = np.array_split(vals, len(vals) // VALUES_PER_ITERATION)

    output = []
    f = 0

    for split in splits:
        print(f"{logFile} {f}   ", end='\r')
        f += 1
        inp = ''.join(list(split)) + "\n-1\n"

        proc = start("./checker")
        out, err = proc.communicate(inp.encode())
        terminate(proc)

        output.append('1' if '1' in out.decode("utf-8") else '0')

    return ''.join(output)


def getHourLog(year,month,date,hour):
    return getCheckerOutputFromLog(f"../data/{year}-{month}-{date}_{hour}.log")

def getDayLog(year,month,date):
    out = ''
    for hour in range(24):
        out += getHourLog(year,month,date,f"{hour:02d}")

    return out

def getPastSevenDaysLogs():
    today = dt.date.today()

    daily = []
    count = 0

    for d in range(7):
        prevDate = today - dt.timedelta(d)
        daily.append(getDayLog(prevDate.year, prevDate.month, prevDate.day))

    return daily
