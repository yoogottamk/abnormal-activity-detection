import matplotlib.pyplot as plt
import numpy as np
import requests

import matplotlib
matplotlib.use("Agg")
SEP = " "

with open("secretdata/BOTUID") as f:
    BOT_UID = f.read().trim()

BOT_URL = f'https://api.telegram.org/{BOT_UID}/'

with open("secretdata/CHAT_IDS") as f:
    CHAT_IDs = f.read().trim()
    if CHAT_IDs:
        CHAT_IDs = CHAT_IDs.split(" ")
    else:
        CHAT_IDs = []


def send_text(msg):
    url = BOT_URL + 'sendMessage'

    for chat_id in CHAT_IDs:
        requests.post(url, json={"chat_id": chat_id, "text": msg})


def send_graph(val, detect, caption):
    url = BOT_URL + 'sendPhoto'

    val = val[:val.find("-1")]

    values = [10*int(x) for x in val.strip().split(SEP)]
    detected_at = np.array([int(x) for x in list(detect.strip())])
    indices = np.where(detected_at == 1)

    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.set_ylim([0, 4095])
    ax.plot(values)

    for idx in indices[0]:
        ax.axvline(x=idx, color='r', linestyle='--', dashes=(1, 10))

    fig.savefig("graph.png")

    for chat_id in CHAT_IDs:
        requests.post(url, data={"chat_id": chat_id, "caption": caption}, files={
                      "photo": open("./graph.png", "rb")})
