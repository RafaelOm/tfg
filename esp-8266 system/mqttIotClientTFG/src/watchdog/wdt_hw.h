/*
 * wdt_hd.h
 *
 *  Created on: 10 nov. 2016
 *      Author: JuanCarlos
 */

#ifndef WDT_HW_H_
#define WDT_HW_H_

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include	<Ticker.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
// Uncomment to enable printing out nice debug messages.
//#define DHT_DEBUG
/*
// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef DHT_DEBUG
  #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define DEBUG_PRINT(...) {}
  #define DEBUG_PRINTLN(...) {}
#endif
*/

#define		TMP_RST_PULSE		1		//Define el ancho del pulse de reset en milisegundos

class WDT_HW{
public:
	WDT_HW(uint8_t pin, uint32_t time_ms);

	void	Rst_Wdt(void);
	void	disable_Wdt(void);
	void 	hw_Rst(void);
	void	Reset_System(void);

private:
	bool		_disabled;
	uint8_t		_pin;
	uint32_t	_time_ms;
	uint32_t	_time_rst;

	uint32_t	lastAct;
	Ticker		_timeforReset;

	static void s_timeforReset(void* arg);
	void	Set_Wdt(void);
};


#endif /* WDT_HW_H_ */
