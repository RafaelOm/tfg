/*
 * wdt_hw.cpp
 *
 *  Created on: 10 nov. 2016
 *      Author: JuanCarlos
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include	"wdt_hw.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define		DEBUG_WD
#define			TMP_WAITING			500		//Time to write . while waiting

WDT_HW::WDT_HW(uint8_t pin, uint32_t time_ms){
	_pin = pin;
	_time_ms = time_ms;
	_disabled = false;

	pinMode(_pin, OUTPUT);
	hw_Rst();					//Emite Pulso de WatchDog reset

	lastAct = millis();
}

/****************************************************************************
 *
 * NAME: stop_Wd
 *
 * DESCRIPTION:
 * Stop Watch Dog. When Stop Watch Dog the system wait for a Hardware Reset
 *
 * RETURNS:
 * none
 *
 ****************************************************************************/
void WDT_HW::Reset_System(void){
	uint32_t	tmp;

	_disabled = true;

	Set_Wdt();

	
	wdt_disable();

	tmp = millis();
	while(1){
		if((millis() - tmp) >= TMP_WAITING){
			Serial.print(".");
			tmp = millis();
		}
		delay(100);

		wdt_reset();
	}
}

/****************************************************************************
 *
 * NAME: Rst_Wdt
 *
 * DESCRIPTION:
 * Reset (inicia) WatchDog Hardware
 *
 * RETURNS:
 * none
 *
 ****************************************************************************/
void WDT_HW::Rst_Wdt(void){

	if((_time_ms <= (millis() - lastAct)) || (millis() < lastAct)){

#ifdef DEBUG_WD
		//DEBUGLOG("_time_ms: %d, millis: %d, lastAct: %d\n\r", _time_ms, millis(), lastAct);
		//DEBUG_PRINTER.println("Rst_Wdt Hardware");
		//DEBUGLOG("Time to Reset: %d\n\r", millis() - lastAct);
#endif

		hw_Rst();				//Emite Pulso de WatchDog reset

		lastAct = millis();
	}
}

/****************************************************************************
 *
 * NAME: disable_Wdt
 *
 * DESCRIPTION:
 * Activa pin inicio para evitar el reset hardware.
 *
 * RETURNS:
 * none
 *
 ****************************************************************************/
void WDT_HW::disable_Wdt(void){
	if(not _disabled){
		digitalWrite(_pin, HIGH);
	}
}

/****************************************************************************
 *
 * NAME: Set_Wdt
 *
 * DESCRIPTION:
 * Disable Pin Reset Watch Dog. WD begin uploading
 *
 * RETURNS:
 * none
 *
 ****************************************************************************/
void WDT_HW::Set_Wdt(void){
	digitalWrite(_pin, LOW);
}

/****************************************************************************
 *
 * NAME: hw_Rst
 *
 * DESCRIPTION:
 * Emite pulso de reset
 *
 * RETURNS:
 * none
 *
 ****************************************************************************/
void WDT_HW::hw_Rst(void){
	_time_rst = millis();
	_timeforReset.attach(1.0f, &WDT_HW::s_timeforReset, static_cast<void*>(this));
	disable_Wdt();
}

void WDT_HW::s_timeforReset(void *arg){
	WDT_HW* self = reinterpret_cast<WDT_HW*>(arg);

	if((millis() - self->_time_rst)>TMP_RST_PULSE){
		self->Set_Wdt();
		(self->_timeforReset).detach();
	}
}


