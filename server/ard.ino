#include <WiFi.h>
#include <HTTPClient.h>

#define sz 3000
short arr[sz];
int c=0;

int pin = 32,
     samplingTime = 100,
     totalTime = 5000,
     n = totalTime / samplingTime,
     val,
     buzzerPin = 25;

String retStr = "";

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
    WiFi.begin("yoogottam", "plis_/\\_plis");

    while(WiFi.status() != WL_CONNECTED) {
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

    if(retCode > 0) {
        Serial.print(successMsg);
        Serial.println(retCode);
        serverResponse = http.getString();

        if(serverResponse.equals("1")) {
            Serial.println("FOUND ANOMALY!!!!");
            digitalWrite(buzzerPin, 1);
            delay(2000);
            digitalWrite(buzzerPin, 0);
        }
    } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(retCode).c_str());
        Serial.print("Some error occured: ");
        Serial.println(retCode);
    }

    http.end();
}

#define READINGS_PER_SECOND 1000
#define DELAY_BW_READINGS 1000 / READINGS_PER_SECOND

void loop() {
    delay(DELAY_BW_READINGS);

    int val = analogRead(pin);
    arr[c++] = val;

    if(c == sz) {
        c = 0;

        for(int i = 0; i < sz; i++) {
            retStr += String(arr[i]);
            retStr += "\n";
        }

        sendString(retStr);
        retStr = "";
    }
}
