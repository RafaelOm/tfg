// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _smartWifiConfig_H_
#define _smartWifiConfig_H_
#include "Arduino.h"
//add your includes for the project smartWifiConfig here
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "LittleFS.h"
#include "ArduinoJson.h"
#include <WiFiClient.h> 
#include "EEPROM_Utils.h"
#include "arduinoPinsMapping.h"
#include "devices.h"
#include "utils.h"
#include "autoConfig.h"
#include "mqtt/mqttConn.h"
#include "watchdog/wdt_hw.h"


/************** Variables Globales*/
extern StaticJsonDocument<500> redesDisponibles;
extern AsyncWebServer server;
extern bool modoConfig;
extern TctrlTime ctrlPorPer;

/**/



/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define 	ON   				HIGH
#define     OFF					LOW

#define    TIME_WDUS            10000            //Watch dog se resetea cada 5 Sec.

//TYPES OF DEVICES
#define ONOFF_DEVICE 1
#define SHUTTER_DEVICE 2
#define BUTTON_ONOFF_DEVICE 3
#define BUTTONS_UPDOWN_DEVICE 4
#define PHOTORESISTANCE_for_OnOff 5

//MODOS DE INICIO

#define FIRST_BOOT 1
#define READY_MODE 2
#define AUTO_CONFIG_MODE 3

//MACROS
//#define SWITCH_AP_STA D1

#define SWITCH_AP_STA 5
#define WATCHDOG_PIN  16


extern int deviceType;

//Do not add code below this line
#endif /* _smartWifiConfig_H_ */
