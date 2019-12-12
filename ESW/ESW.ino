#include <WiFi.h>
#include <HTTPClient.h>

#define READINGS_PER_SECOND 400
#define DELAY_BW_READINGS 1000000 / READINGS_PER_SECOND
#define sz 3 * READINGS_PER_SECOND

short arr[sz];
int c = 0;

int pin = 32, samplingTime = 100, totalTime = 5000,
    n = totalTime / samplingTime, val, buzzerPin = 25;

String retStr = "";
String cse_ip = "139.59.42.21";

HTTPClient http;

void setup() {
    Serial.begin(9600);
    pinMode(buzzerPin, OUTPUT);

    /**
     * Setting up WiFi connection
     *
     * TODO: Use WiFiMulti maybe
     */
    Serial.print("Connecting to WiFi");
    WiFi.begin("esw-m19@iiith", "e5W-eMai@3!20hOct");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

String serverResponse;
// please put forward slash in the ending
String serverURL = "http://10.2.8.12:9999/";
char* successMsg = "SUCCESS: ";
void sendString(String str) {
    Serial.println("Sending data: ");

    http.begin(serverURL);
    int retCode = http.POST(str);

    if (retCode > 0) {
        Serial.print(successMsg);
        Serial.println(retCode);
        serverResponse = http.getString();

        if (serverResponse.equals("1")) {
            Serial.println("FOUND ANOMALY!!!!");
            digitalWrite(buzzerPin, 1);
            delay(2000);
            digitalWrite(buzzerPin, 0);
        }
    } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n",
                      http.errorToString(retCode).c_str());
        Serial.print("Some error occured: ");
        Serial.println(retCode);
    }

    http.end();
}

String normalize(int x) {
    if (x < 10)
        return "00" + String(x);
    else if (x < 100)
        return "0" + String(x);
    else
        return String(x);
}

String cse_port = "8080";
String server = "http://" + cse_ip + ":" + cse_port + "/~/in-cse/in-name/";

String createCI(String server, String ae, String cnt, String val) {
    HTTPClient http;
    http.begin(server + ae + "/" + cnt + "/");

    http.addHeader("X-M2M-Origin", "admin:admin");
    http.addHeader("Content-Type", "application/json;ty=4");
    http.addHeader("Content-Length", "100");
    http.addHeader("Connection", "close");
    int code = http.POST("{\"m2m:cin\": {\"cnf\": \"text/plain:0\",\"con\": " +
                         String(val) + "}}");
    http.end();
    Serial.print(code);
    delay(300);
    return "";
}

void loop() {
    delayMicroseconds(DELAY_BW_READINGS);

    int val = analogRead(pin);
    arr[c++] = val;

    if (c == sz) {
        c = 0;

        for (int i = 0; i < sz; i++) {
            retStr += normalize(arr[i] / 10);
        }

        retStr += " -1 ";
        sendString(retStr);
        retStr = "";
    }

    // String sensor1 = "(" + String(val) + "," + String(h) + ")";

    // when single sensor gives single value
    String sensor2 = String(val);

    // Make it a single string
    String sensor_string = sensor2;

    // Make it OneM2M complaint
    sensor_string = "\"" + sensor_string + "\"";  // DO NOT CHANGE THIS LINE

    // Send data to OneM2M server
    createCI(server, "Team34_Abnormal_activity_detection_outside_of_classroom",
             "node_1", sensor_string);

    delay(1500);  // DO NOT CHANGE THIS LINE
}
