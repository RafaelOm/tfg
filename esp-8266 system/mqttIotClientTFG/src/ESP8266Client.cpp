// Do not remove the include below
#include "ESP8266Client.h"
#include "serverConf.h"




unsigned long timeNow=0;
bool modoConfig =false;
int deviceType;

StaticJsonDocument<500> redesDisponibles;
 WDT_HW *wdt_hw;
 


void setup(){
   wdt_hw = new WDT_HW(WATCHDOG_PIN, TIME_WDUS);
  // Serial port for debugging purposes
  Serial.begin(115200);
  EEPROM.begin(512);
  // Initialize SPIFFS
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

    pinMode(SWITCH_AP_STA, INPUT); //pulsador


  if(digitalRead(SWITCH_AP_STA)){//leo pulsador
      modoConfig =true;
      Devices::setSmartConfigMode();
      setServer();
      setWifiAPMODE();
  }else if(leerEEPROMbyte(CONFIGURACION_INICIAL)==READY_MODE){
    connectToWifi();
    configMQTT();
    Devices::setCorrectMode();
    deviceType=std::atoi(leerEEPROM(DEVICE_TYPE,DEVICE_TYPElength).c_str());
    confDevices();
    Serial.println(leerEEPROM(MQTT_ROUTE_STATUS,MQTT_ROUTE_STATUSlength));
    
 
  }else if (leerEEPROMbyte(CONFIGURACION_INICIAL)==AUTO_CONFIG_MODE){
      deviceType=std::atoi(leerEEPROM(DEVICE_TYPE,DEVICE_TYPElength).c_str());
      connectToWifi();
      autoConfig();
      Devices::setSmartConfigMode();

  }



}
 

void loop(){
 
   wdt_hw->Rst_Wdt();
  if(millis()>=timeNow+4000 && modoConfig){
    buscarRedes();
    timeNow = millis();
  }else if (leerEEPROMbyte(CONFIGURACION_INICIAL)==READY_MODE && !modoConfig){
    initMqtt();
    if(deviceType!=PHOTORESISTANCE_for_OnOff){
        devices.leerDigital();
    }else if (millis()>= timeNow+1000){
        devices.leerAnalog();
        timeNow = millis();
    }
    
  }else if (leerEEPROMbyte(CONFIGURACION_INICIAL)==AUTO_CONFIG_MODE && !modoConfig){
    initMqttAutoConfig();
  }
  //leerEEPROM(PASSWORD_EEPROM,PASSWORDlengt).toCharArray(passEP,50);
   
}
