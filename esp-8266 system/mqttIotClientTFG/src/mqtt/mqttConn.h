#include <Arduino.h>
#include "PubSubClient.h"
#include <ESP8266WiFi.h>
#include "devices.h"
#include "utils.h"
#include <string>
#include <iostream>
#include "IPAddress.h"



void initMqtt();
void configMQTT();
extern tsStaPer stateRegister ;
extern String deviceId;

void initMqttAutoConfig();
String randomName();
void autoConfig();
void publishStatus();
void subscribeTopic();
void publishToMQTT(const char* value);
void autoConfigSensor(const char* value);