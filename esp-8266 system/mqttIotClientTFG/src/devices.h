/*
 * devices.h
 *
 *  Created on: 15 oct. 2021
 *      Author: Rafael
 */

#ifndef DEVICES_H_
#define DEVICES_H_

#include "Arduino.h"



//---------------------CONEXION PINES---------------

#define builtLed_P 2

/*#define ledCorrecto_P 13		//D7
#define ledError_P 15			//D8
#define ledSmartConfig_P 12		//D6
#define perONOFF_P 14			//D5
#define perUPDOWN_P 16			//D0
*/
#define ledCorrecto_P 15		
#define ledError_P 13			
#define ledSmartConfig_P 2		
#define perONOFF_P 14			
#define perUPDOWN_P 12

#define button1_P  4
#define button2_P  5

#define photoresistance_P  A0


//---------------------------------------------------
typedef enum
{
	S_DIGITAL,
	S_ANALOGICO
}teSensor;

typedef struct
{
	byte	pin;
	byte	estado;
}tsActuator;

typedef enum {
	PER_STOP,
	PER_DOWN,
	PER_UP,
	PER_STOP2
}tsStaPer; //Control persianas

//Estructura para controlar los tiempos/porcentajes de activaci�n (Persiana)
typedef struct{
	bool			activa;
	uint32_t 		time_lastAct;		//�ltima actualizaci�n del tiempo
	uint16_t		*maxTime;			//puntero al tiempo m�ximo de activaci�n
	int32_t			*actTime;			//puntero al tiempo actual de activaci�n
}TctrlTime, *TctrlTimePtr;

typedef struct{
	int limiteInferior;
	int limiteSuperior;

}TctrlLuminosidad, *TctrlLuminosidadPtr;

typedef struct
{
	byte				pin;			//Pin asignado al sensor
	int	    			valor;			//Valor leido
	int					valor_Df;		//Valor Sensor por defecto
	teSensor			eSensor;		//Tipos de sensor ANAL�GICO/DIGITAL
	int					Aux;			//Variables para ser usadas en la gesti�n del sensor
	String				name;
	TctrlLuminosidadPtr ctrlLuminosidadPtr;//Para Controlar el momento en el que enciende un actuador
}TSensor, *TSensorPtr;

typedef void (*TSensorEvent)(TSensor *);

typedef struct
{
	TSensor				Sensor;
	TSensorEvent		SensorEvent;	//Evento para aplicaci�n asociada
}PTSensor, *PTSensorPtr;

class Devices
{
private:


public:

	Devices(); 						//Constructor
	tsActuator builtLed;
	tsActuator ledCorrecto;
	tsActuator ledError;
	tsActuator ledSmartConfig;
	tsActuator PER_ONOFF;
	tsActuator PER_UPDOWN;

	PTSensor		BUTTON_1;
	PTSensor		BUTTON_2;

	PTSensor		PHOTORESISTANCE;

	static void	setActuator(tsActuator *Actuator, bool val);
	static void setSmartConfigMode();
	static void setErrorMode();
	static void setCorrectMode();
	void  SetPersiana(tsStaPer staPer);
	void  leerDigital(void);
	void  leerAnalog(void);
	static void 	leerSensor(PTSensorPtr Sensor);
};

extern Devices devices;




#endif /* DEVICES_H_ */
