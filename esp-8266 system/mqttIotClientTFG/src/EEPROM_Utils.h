
#include <EEPROM.h>
#include <Arduino.h>

void guardarEEPROM(int addr, String dato,int EEPROMSIZE);
String leerEEPROM(int addr,int EEPROMSIZE);

void guardarEEPROMbyte(int addr, int dato);
int leerEEPROMbyte(int addr);


//***************MACRO DEFINITIONS ****************

#define SSID_EEPROM 0   // 0-49
#define PASSWORD_EEPROM 50  //50-99
#define CONFIGURACION_INICIAL 100  //100-104
#define STATUS_PERSIANA 105         //105-109
#define PERCENTAGE_PERSIANA 110     //110-114
#define ACTIVETIME_PERSIANA 115     //115-119
#define MQTTSERVERIP 120 //         //120-134
#define MQTT_ROUTE_MOVE 135 //         //135-234
#define MQTT_ROUTE_STATUS 235 //         //235-334
#define DEVICE_TYPE 335                 //335-339
#define ON_OFF_DEVICE_STATUS 340        //340
#define PHOTORESISTANCE_UPPER_LIMIT 341 //341 - 343
#define PHOTORESISTANCE_LOWER_LIMIT 344 //344 - 346

#define SSIDlengt 50 //
#define PASSWORDlengt 50 //
#define STATUS_PERSIANAlength 5 //
#define PERCENTAGE_PERSIANAlength 5 //
#define ACTIVETIME_PERSIANAlenght 5
#define MQTTSERVERIPlength 15 //         //119-134
#define MQTT_ROUTE_MOVElength    100
#define MQTT_ROUTE_STATUSlength    100
#define DEVICE_TYPElength 5
#define ON_OFF_DEVICE_STATUSlength 1
#define PHOTORESISTANCE_UPPER_LIMITlength 3
#define PHOTORESISTANCE_LOWER_LIMITlength 3





