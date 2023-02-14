/*
 * devices.cpp
 *
 *  Created on: 15 oct. 2021
 *      Author: Rafael
 */

#include"devices.h"

Devices::Devices()
{
	pinMode(builtLed_P, OUTPUT);
	pinMode(ledError_P, OUTPUT);
	pinMode(ledCorrecto_P, OUTPUT);
	pinMode(ledSmartConfig_P, OUTPUT);

	builtLed.pin 					= builtLed_P;
	builtLed.estado 				= LOW;

	ledCorrecto.pin 				=ledCorrecto_P;
	ledCorrecto.estado				=LOW; 

	ledError.pin					=ledError_P;
	ledError.estado 				=LOW;

	ledSmartConfig.pin				=ledSmartConfig_P;
	ledSmartConfig.estado			=LOW;
	
	PER_ONOFF.pin					=perONOFF_P;
	PER_ONOFF.estado				=LOW;
	pinMode(perONOFF_P,OUTPUT);

	PER_UPDOWN.pin					=perUPDOWN_P;
	PER_UPDOWN.estado	=			LOW;
	pinMode(perUPDOWN_P,OUTPUT);

	BUTTON_1.Sensor.pin	= button1_P;
	BUTTON_1.Sensor.valor= LOW;
	BUTTON_1.Sensor.valor_Df =LOW;
	BUTTON_1.Sensor.eSensor = S_DIGITAL;
	BUTTON_1.Sensor.name = "BUTTON 1";
	BUTTON_1.SensorEvent = NULL;
	pinMode(button1_P, INPUT);

	BUTTON_2.Sensor.pin	= button2_P;
	BUTTON_2.Sensor.valor= LOW;
	BUTTON_2.Sensor.valor_Df =LOW;
	BUTTON_2.Sensor.eSensor = S_DIGITAL;
	BUTTON_2.Sensor.name = "BUTTON 2";
	BUTTON_2.SensorEvent = NULL;
	pinMode(button2_P, INPUT);

	PHOTORESISTANCE.Sensor.pin=photoresistance_P;
	PHOTORESISTANCE.Sensor.eSensor=S_ANALOGICO;
	PHOTORESISTANCE.Sensor.name ="photoresistance";

}

void  Devices::setActuator(tsActuator *Actuator, bool val){
	digitalWrite(Actuator->pin, val);
	Actuator->estado = val;
}

void  Devices::setSmartConfigMode(){
		setActuator(&devices.ledSmartConfig,HIGH);
		setActuator(&devices.ledError,LOW);
		setActuator(&devices.ledCorrecto,LOW);


 }
void  Devices:: setErrorMode(){
		setActuator(&devices.ledSmartConfig,LOW);
		setActuator(&devices.ledError,HIGH);
		setActuator(&devices.ledCorrecto,LOW);
}
void  Devices:: setCorrectMode(){
		setActuator(&devices.ledSmartConfig,LOW);
		setActuator(&devices.ledError,LOW);
		setActuator(&devices.ledCorrecto,HIGH);

}

void  Devices::SetPersiana(tsStaPer staPer)
{
	switch(staPer){
	case PER_STOP:
	case PER_STOP2:
		setActuator(&PER_ONOFF, false);
		setActuator(&PER_UPDOWN, false);
		break;

	case PER_DOWN:
		setActuator(&PER_UPDOWN, false);
		setActuator(&PER_ONOFF, true);
		break;

	case PER_UP:
		setActuator(&PER_UPDOWN, true);
		setActuator(&PER_ONOFF, true);
		break;
	}
}

void Devices::leerDigital(void){
	leerSensor(&BUTTON_1);
	leerSensor(&BUTTON_2);
}
void Devices::leerAnalog(void){
	leerSensor(&PHOTORESISTANCE);   
}
void  Devices::leerSensor(PTSensorPtr Sensor){
	int valor = 0;

	
		switch (Sensor->Sensor.eSensor){
		case S_DIGITAL:
			valor = digitalRead(Sensor->Sensor.pin);
			break;

		case S_ANALOGICO:
			valor = analogRead(Sensor->Sensor.pin);
			break;
		}

		if(Sensor->Sensor.valor != valor){
			Sensor->Sensor.valor = valor;

			//Si hay un cambio en el estado del sensor llamamos a la aplicaci�n asociada
			if(Sensor->SensorEvent != NULL)
				Sensor->SensorEvent(&(Sensor->Sensor));
		}
	
}

Devices devices;


