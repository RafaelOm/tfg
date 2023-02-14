#include "autoConfig.h"

char ssidEP[50];
char passEP[50];

void connectFromAP(){
    WiFi.softAPdisconnect(true);
    server.end();
    modoConfig=false;
    connectToWifi();
    
}

void connectToWifi(){

    leerEEPROM(SSID_EEPROM,SSIDlengt).toCharArray(ssidEP,50);
    leerEEPROM(PASSWORD_EEPROM,PASSWORDlengt).toCharArray(passEP,50);
    Serial.println(ssidEP);
    Serial.println(passEP);
     delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssidEP);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssidEP, passEP);

 Devices::setErrorMode();
  while (WiFi.status() != WL_CONNECTED) {      //NO FUNCION REVISAR
    delay(500);
    Serial.print(".");
   
  }
  Devices::setCorrectMode();

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  //Serial.println("Potencia: "+WiFi.RSSI());
  Serial.println(WiFi.localIP());
}

