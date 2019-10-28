#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>

String urlencode(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
    
}

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
    WiFi.begin("yoogottam", "_plis_/\\_plis_");

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
String serverURL = "http://25b4fcde.ngrok.io/test/";
char* successMsg = "SUCCESS: ";
void sendString(String str) {
    Serial.println("Sending data: ");
    
    // somehow, the subprcoess call is taking a lot of time
//    http.setConnectTimeout(20000);
    //    http.setTimeout(20000);

//    str = "data=" + urlencode(str);
    http.begin(serverURL);
    int retCode = http.GET();

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
   /*
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
    */

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
