  
  
  #include "serverConf.h"
  #include "ESP8266Client.h"

    const char* ssid = "dispotivoIot";
    const char* password = "12345678";  
    AsyncWebServer server(80);
  void setServer(){
      // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html");
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/style.css", "text/css");
  });


    
  server.on("/sendInfo", HTTP_POST, [](AsyncWebServerRequest *request){
    
    
    //Serial.print(request->getParam(0)->value());
    guardarEEPROM(SSID_EEPROM,request->getParam(0)->value(),SSIDlengt);
    //Serial.print(request->getParam(1)->value());
    guardarEEPROM(PASSWORD_EEPROM,request->getParam(1)->value(),PASSWORDlengt);
    guardarEEPROM(MQTTSERVERIP,request->getParam(2)->value(),MQTTSERVERIPlength);
    if(request->getParam(4)->value()=="smart"){
      guardarEEPROMbyte(CONFIGURACION_INICIAL,AUTO_CONFIG_MODE);
      guardarEEPROM(DEVICE_TYPE,request->getParam(3)->value(),DEVICE_TYPElength);
    }else{      
      guardarEEPROMbyte(CONFIGURACION_INICIAL,READY_MODE);
      guardarEEPROM(MQTT_ROUTE_MOVE,request->getParam(5)->value().c_str(),MQTT_ROUTE_MOVElength);
      guardarEEPROM(MQTT_ROUTE_STATUS,request->getParam(6)->value().c_str(),MQTT_ROUTE_STATUSlength);
      guardarEEPROM(DEVICE_TYPE,request->getParam(3)->value(),DEVICE_TYPElength);
      //AÑADIR LINEAS DE RUTAS MQTT
    }    
     ESP.reset();
    
  });
      
  server.on("/getWifiAvaliable", HTTP_GET, [](AsyncWebServerRequest *request){
    String response;
    serializeJson(redesDisponibles, response);
    request->send(200, "application/json",response);
  });

  server.begin();
  }

void setWifiAPMODE(){
  WiFi.softAP(deviceId, password);
  IPAddress myIP = WiFi.softAPIP(); 
  Serial.print("IP del acces point: ");
  Serial.println(myIP);
  Serial.println("WebServer iniciado...");
  Serial.println(deviceId);

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}
  