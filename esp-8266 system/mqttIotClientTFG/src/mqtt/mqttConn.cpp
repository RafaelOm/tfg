
#include "mqttConn.h"
#include <iostream>


const char* mqtt_server = "192.168.1.130";
String  deviceId = String("TFG_")+String (ESP.getChipId());                        
unsigned long elapsedTime=0;

WiFiClient espClient;
PubSubClient client(espClient);

tsStaPer stateRegister = PER_STOP;
String randomDeviceName;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  String value;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    value  += (char)payload[i];
  }
  Serial.println("------------MENSAJE RECIBIDO");

  if(leerEEPROMbyte(CONFIGURACION_INICIAL)==AUTO_CONFIG_MODE){
     if(strcmp(value.c_str(),"addDevice")==0){//Mensaje de dispositivo encontrado
       Serial.println("Conectando a nuevo servidor WEB");
       String routeMove = deviceId+"/move";
       guardarEEPROM(MQTT_ROUTE_MOVE,routeMove.c_str(),MQTT_ROUTE_MOVElength);
        String routeStatus = deviceId+"/status";
       guardarEEPROM(MQTT_ROUTE_STATUS,routeStatus.c_str(),MQTT_ROUTE_STATUSlength);
       guardarEEPROMbyte(CONFIGURACION_INICIAL,READY_MODE);
      }else if (deviceType != ONOFF_DEVICE && deviceType != SHUTTER_DEVICE){  //configuracion de sensor
         autoConfigSensor(value.c_str());
      }
     ESP.reset();

  }else{
      ctrlDevices(value.c_str());
  }
      

  

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(deviceId.c_str())) {
      Serial.println("connected");

      if(leerEEPROMbyte(CONFIGURACION_INICIAL)==AUTO_CONFIG_MODE){
        client.subscribe(deviceId.c_str());
      }else{
          subscribeTopic();
        
      }
      
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void configMQTT(){
  
   
    Serial.print("host="); Serial.println(leerEEPROM(MQTTSERVERIP,MQTTSERVERIPlength));
    IPAddress ip;
    ip.fromString(leerEEPROM(MQTTSERVERIP,MQTTSERVERIPlength));
	  client.setServer(ip, 1883);
	  client.setCallback(callback);
    
 

}

void initMqtt(){
    if (!client.connected()) {
	    reconnect();
	  }
	  client.loop();
    if(millis()>=elapsedTime+1000){
     publishStatus();
      elapsedTime =millis();
    }
    
}

void publishStatus(){
    switch(deviceType){

        case ONOFF_DEVICE:
            if(devices.builtLed.estado==LOW){
               client.publish(leerEEPROM(MQTT_ROUTE_STATUS,MQTT_ROUTE_STATUSlength).c_str(),"on");
            }else{
               client.publish(leerEEPROM(MQTT_ROUTE_STATUS,MQTT_ROUTE_STATUSlength).c_str(),"off");
            }
            Serial.println(devices.builtLed.estado);
          
            break;
        case SHUTTER_DEVICE:
			    Ctrl_PosicionPersiana(&ctrlPorPer,stateRegister);
          client.publish(leerEEPROM(MQTT_ROUTE_STATUS,MQTT_ROUTE_STATUSlength).c_str(),std::to_string(percentage).c_str());
            break;
          }

}

void subscribeTopic(){
  switch(deviceType){

        case ONOFF_DEVICE:
          client.subscribe(leerEEPROM(MQTT_ROUTE_STATUS,MQTT_ROUTE_STATUSlength).c_str());
            break;
        case SHUTTER_DEVICE:
            client.subscribe(leerEEPROM(MQTT_ROUTE_MOVE,MQTT_ROUTE_MOVElength).c_str());
            break;
        case BUTTON_ONOFF_DEVICE:
            client.subscribe(leerEEPROM(MQTT_ROUTE_STATUS,MQTT_ROUTE_STATUSlength).c_str());
              break;
          }



}
//llamada al configurar dispositivo MQTT
void autoConfig(){

    configMQTT();
    randomDeviceName = randomName();
    client.subscribe(deviceId.c_str());
  

}

void initMqttAutoConfig(){
    if (!client.connected()) {
	    reconnect();
	  }
	  client.loop();
    if(millis()>=elapsedTime+1000){
      String autoConfigPayload= deviceId+ String("-")+ String(deviceType);
      client.publish("autoConfig",autoConfigPayload.c_str());
      elapsedTime =millis();
    }
    
}

void publishToMQTT(const char* value){
  

    switch(deviceType){
      case BUTTON_ONOFF_DEVICE:
          client.publish(leerEEPROM(MQTT_ROUTE_STATUS,MQTT_ROUTE_STATUSlength).c_str(),value);
          break;
      case BUTTONS_UPDOWN_DEVICE:
          client.publish(leerEEPROM(MQTT_ROUTE_MOVE,MQTT_ROUTE_MOVElength).c_str(),value);
          break;
      case PHOTORESISTANCE_for_OnOff:
      Serial.println(leerEEPROM(MQTT_ROUTE_STATUS,MQTT_ROUTE_STATUSlength).c_str());
            client.publish(leerEEPROM(MQTT_ROUTE_STATUS,MQTT_ROUTE_STATUSlength).c_str(),value);      
          break;
    }
}

void autoConfigSensor(const char* value){

      String routeMove = "";
      String routeStatus="";
        switch (deviceType){
        case BUTTON_ONOFF_DEVICE:
            guardarEEPROM(MQTT_ROUTE_MOVE,routeMove.c_str(),MQTT_ROUTE_MOVElength);
            guardarEEPROM(MQTT_ROUTE_STATUS,value,MQTT_ROUTE_STATUSlength);
            guardarEEPROMbyte(CONFIGURACION_INICIAL,READY_MODE);
          break;
        
        case BUTTONS_UPDOWN_DEVICE:
            guardarEEPROM(MQTT_ROUTE_MOVE,value,MQTT_ROUTE_MOVElength);
            guardarEEPROM(MQTT_ROUTE_STATUS,routeStatus.c_str(),MQTT_ROUTE_STATUSlength);
            guardarEEPROMbyte(CONFIGURACION_INICIAL,READY_MODE);
          break;
        case PHOTORESISTANCE_for_OnOff:
            guardarEEPROM(MQTT_ROUTE_MOVE,routeMove.c_str(),MQTT_ROUTE_MOVElength);
            splitAndSaveMqttPayload(value);
            guardarEEPROMbyte(CONFIGURACION_INICIAL,READY_MODE);
        break;
        }
}

String randomName(){

  static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    String tmp_s;
    tmp_s.reserve(10);

    for (int i = 0; i < 10; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;

}