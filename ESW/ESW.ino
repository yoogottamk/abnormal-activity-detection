#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define sz 3000
short arr[sz];
int c=0;

/*
String fileName = "/out.txt";
File file;
*/

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
    /*
    if(!SD.begin(5)){ // the five is the CS pin
        Serial.println("Card Mount Failed");
        return;
    }

    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    SD.remove(fileName);
    file = SD.open(fileName, FILE_WRITE);
    if (!file) {
        Serial.println("File couldn't open");
        return;
    }
    */

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

void sendString(String str) {
    Serial.println("Sending data: ");
    http.begin("http://3188385a.ngrok.io/");

    int retCode = http.POST(str);

    if(retCode > 0) {
        Serial.print("SUCCESS: ");
        Serial.println(retCode);
        String x = http.getString();
        Serial.println(x);
        if(x.equals("0"))
            digitalWrite(buzzerPin, 1);
        else
            Serial.println("No");
    } else {
        Serial.print("Some error occured: ");
        Serial.println(retCode);
    }
    http.end();
}

void loop() {
    n--;
    if(n <= 0) {
        if(n == 0) {
            for(int i = 0; i < c; i++) {
                // file.println(arr[i]);
                retStr += String(arr[i]);
                retStr += ",";
            }

            sendString(retStr);
            retStr = "";
            Serial.println("Finished!");
//            file.close();
        }
        return;
    }

    unsigned long start = millis();

    while(millis() - start <= samplingTime) {
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
}
