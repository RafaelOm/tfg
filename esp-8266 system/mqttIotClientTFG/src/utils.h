#include "ESP8266Client.h"

void buscarRedes();

void UpDown_Persiana(tsStaPer state,bool UpP, bool DownP);
void Ctrl_PosicionPersiana(TctrlTimePtr ctrlPosPer, tsStaPer staPer);
void confPersiana();
void confDevices();
void ctrlDevices(const char *value);
void ctrlPersiana(const char *value);
void ctrlOnOffDevice(const char *value);
void ctrlButtonOnOff(TSensorPtr Sensor);
void ctrlButtonUpDown(TSensorPtr Sensor);
void ctrlPhotoresistance(TSensorPtr Sensor);
void confButtonOnOff();
void confOnOffDevice();
void confButtonUpDown();
void confPhotoresistance();
void splitAndSaveMqttPayload(const char* value);
extern uint8_t percentage;
