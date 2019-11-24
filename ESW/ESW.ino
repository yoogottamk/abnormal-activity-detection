#include <WiFi.h>
#include <HTTPClient.h>

#define READINGS_PER_SECOND 400
#define DELAY_BW_READINGS 1000000 / READINGS_PER_SECOND
#define sz 3 * READINGS_PER_SECOND

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
    WiFi.begin("esw-m19@iiith", "e5W-eMai@3!20hOct");

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

String normalize(int x){
    if(x<10) return "00"+String(x);
    else if(x<100) return "0" + String(x);
    else return String(x);
}

void loop() {
    delayMicroseconds(DELAY_BW_READINGS);

    int val = analogRead(pin);
    arr[c++] = val;

    if(c == sz) {
        c = 0;

        for(int i = 0; i < sz; i++) {
            retStr += normalize(arr[i] / 10);
        }

        retStr += " -1 ";
        sendString(retStr);
        retStr = "";
    }
}
