import os
import datetime as dt
import numpy as np

OUTPUT_LOCATION = "../data/outputs/"

def getDailyCount(year, month, date):
    fileName = f"{OUTPUT_LOCATION}{year}-{month}-{date}.log"

    if not os.path.exists(fileName):
        return 0

    with open(fileName, "r") as f:
        text = f.read()

    return text.count("1")

def getWeeklyCount(year, month, firstDayOfWeek):
    firstDay = dt.datetime(year=year, month=month, day=firstDayOfWeek)
    count = 0

    for d in range(7):
        newDate = firstDay + dt.timedelta(days=d)
        count += getDailyCount(newDate.year, newDate.month, newDate.day)

    return count

def getQuarterCount(year, month, date):
    fileName = f"{OUTPUT_LOCATION}{year}-{month}-{date}.log"

    if not os.path.exists(fileName):
        print(f"The file {fileName} doesn't exist (yet)")
        return 0

    with open(fileName, "r") as f:
        text = f.read()

    quarters = np.array(list(text))
    cnts = np.array_split(quarters, 4)

    ret = []

    for cnt in cnts:
        ret.append(np.count_nonzero(cnt == "1"))

    return ret
