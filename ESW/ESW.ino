#include <FS.h>
#include <SD.h>
#include <SPI.h>

long
    pin = 12,
    samplingTime = 100,
    totalTime = 5000,
    n = totalTime / samplingTime,
    val;

String fileName = "/out.txt";
File file;

void setup() {
  Serial.begin(9600);
  
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
}

#define sz 50000
short arr[sz];
int c=0;

void loop() {
  n--;
  if(n <= 0) {
    if(n==0){
      Serial.println("Finished!");
      file.close();      
    }
    return;
  }

  unsigned long start = millis();

  while(millis() - start <= samplingTime) {
      short val = analogRead(pin);
      arr[c++] = val;
      if(c == sz) {
        c = 0;
        
        for(int i = 0; i < sz; i++) file.println(arr[i]);
      
        Serial.println("Writing to file");  
      }
//      Serial.println(val);
//      file.println(val); expensive $$
  }
}
