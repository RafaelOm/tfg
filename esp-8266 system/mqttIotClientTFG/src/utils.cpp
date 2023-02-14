#include "utils.h"

#define	STEP_CTRL_BLIND		5
#define PERCENTAGE(x)		((x % STEP_CTRL_BLIND) > 2) ? x + (STEP_CTRL_BLIND-(x % STEP_CTRL_BLIND)) : x - (x % STEP_CTRL_BLIND);

uint8_t percentage;

const uint16_t	persianaTiempoSubida 	= 7200;
TctrlTime 	ctrlPorPer;			//Control del tanto porciento de apertura de la parsiana
String onOffDeviceStatus="";

 void buscarRedes(){
  int n = WiFi.scanNetworks(); 
  StaticJsonDocument<500> data;
    for (int i = 0; i < n; i++){
      data.add(WiFi.SSID(i));
      //Serial.print(WiFi.SSID(i));
    }
    redesDisponibles=data;
}

/*============================================*/
/*			     Control Persiana             */
/*============================================*/
void UpDown_Persiana(tsStaPer state,bool UpP, bool DownP){
	

    switch(state){
    	case PER_STOP: //Parada
    		if (UpP == ON) {
    			state = PER_UP; //Subiendo
    		}

    		if (DownP == ON) {
    			state = PER_DOWN; //Bajando
    		}

    		break;

    	case PER_UP: //Subiendo
    		if(!UpP && !DownP) {
    			state = PER_STOP;
    		}

    		if(!UpP && DownP){
    			state = PER_STOP;
    		}
    		break;

    	case PER_DOWN: //Bajando
    		if(!UpP && !DownP) {
    			state = PER_STOP;
    		}

    		if(UpP && !DownP){
    			state = PER_STOP;
    		}
    		break;

    	case PER_STOP2:
    		if(!UpP && !DownP) {
    			state = PER_STOP;
    		}
    		break;

    }

      
	
	 if(stateRegister != state){
		 stateRegister = state;
		devices.SetPersiana(state);
    }

	
}



void Ctrl_PosicionPersiana(TctrlTimePtr ctrlPosPer, tsStaPer staPer){
	static tsStaPer lastStePer = PER_STOP;

	// lambda function
	auto actualizeTime = [&](int8_t sign){

		*ctrlPosPer->actTime = *ctrlPosPer->actTime + sign*(millis() - ctrlPosPer->time_lastAct);

		if(*ctrlPosPer->actTime < 0){
			*ctrlPosPer->actTime = 0;
		}else if(uint16_t(*ctrlPosPer->actTime) > *ctrlPosPer->maxTime){
			*ctrlPosPer->actTime = *ctrlPosPer->maxTime;
		}
		guardarEEPROM(ACTIVETIME_PERSIANA,std::to_string(*ctrlPosPer->actTime).c_str(),ACTIVETIME_PERSIANAlenght);
		percentage = PERCENTAGE((*ctrlPosPer->actTime)*100/(*ctrlPosPer->maxTime));//DEVOLVER POR MQTT
		guardarEEPROM(PERCENTAGE_PERSIANA,std::to_string(percentage).c_str(),PERCENTAGE_PERSIANAlength);
		
		Serial.println(std::atoi(leerEEPROM(PERCENTAGE_PERSIANA,PERCENTAGE_PERSIANAlength).c_str()));
		if(percentage ==100 || percentage ==0){ //PARAMOS LA PERSIANA 
			UpDown_Persiana(stateRegister,false,false);
		}
		Serial.println(percentage);
	};

	switch(staPer){
	case PER_STOP:
	case PER_STOP2:
		ctrlPosPer->activa = false;
		switch(lastStePer){
		case PER_DOWN:
			actualizeTime(-1);
			break;
		case PER_UP:
			actualizeTime(1);
			break;
		default:
			break;
		}
		break;

	case PER_DOWN:
		if(ctrlPosPer->activa){
			//Continúa bajado
			actualizeTime(-1);
		}else{
			//Comienza a bajada
			ctrlPosPer->activa = true;
		}
		ctrlPosPer->time_lastAct = millis();
		break;

	case PER_UP:
		if(ctrlPosPer->activa){
			//Continúa subiendo
			actualizeTime(1);
		}else{
			//Comienza a subir
			ctrlPosPer->activa = true;
		}
		ctrlPosPer->time_lastAct = millis();
		break;
	}

	lastStePer = staPer;
}

void ctrlDevices(const char *value){

	switch(deviceType){
        case ONOFF_DEVICE:
				ctrlOnOffDevice(value);
              break;
        case SHUTTER_DEVICE:
				ctrlPersiana(value);
              break;
        case BUTTON_ONOFF_DEVICE:
				onOffDeviceStatus=String (value);
            break;
		case BUTTONS_UPDOWN_DEVICE:
			break;
          }
}

void ctrlPersiana(const char *value){

	if(strcmp(value,"0")==0){//Bajar persiana
       Serial.println("bajar persiana:");
      Serial.println(stateRegister);
         UpDown_Persiana(stateRegister,false,true);
      }else if(strcmp(value,"1")==0) {
         Serial.println("subir persiana:");
          Serial.println(stateRegister);
         UpDown_Persiana(stateRegister,true,false);
      }else{
           Serial.println("parada");
          Serial.println(stateRegister);
         UpDown_Persiana(stateRegister,false,false);
      }
}

void ctrlOnOffDevice(const char *value){
	  
  // Switch on the LED if an 1 was received as first character
  if (strcmp(value,"on")==0) {
	  	Devices::setActuator(&devices.builtLed, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
	
		guardarEEPROMbyte(ON_OFF_DEVICE_STATUS,LOW);
  } else {
	  	Devices::setActuator(&devices.builtLed, HIGH);  // Turn the LED off by making the voltage HIGH
		guardarEEPROMbyte(ON_OFF_DEVICE_STATUS,HIGH);
  }
	Serial.print("NEW VALUE: ");
	Serial.println(leerEEPROMbyte(ON_OFF_DEVICE_STATUS));
	

}


void ctrlButtonOnOff(TSensorPtr Sensor){
	String valor ;
	Serial.print(onOffDeviceStatus);
	if(Sensor->valor == ON){
		if(onOffDeviceStatus =="on"){ //Revertimos el estado del dispositivo remoto
			valor ="off";
		}else{
			valor ="on";
		}
			publishToMQTT(valor.c_str());
	}

	


}
void ctrlButtonUpDown(TSensorPtr Sensor){

	Serial.print(Sensor->name);
	if(Sensor->valor_Df != Sensor->valor){//pulsador pulsado
		String action = (Sensor->name == "UP") ? "1" : "0";
		publishToMQTT(action.c_str());
		Serial.print(action);
	}

}

void ctrlPhotoresistance(TSensorPtr Sensor){
	int valor =analogRead(Sensor->pin);
	int inf  =Sensor->ctrlLuminosidadPtr->limiteInferior;
	int sup =Sensor->ctrlLuminosidadPtr->limiteSuperior;
		Serial.println(valor);
		if(valor<inf && Sensor->Aux !=1 ){
			publishToMQTT("on");
			Sensor->Aux =1;

		}else if(valor>sup && Sensor->Aux !=0){
			publishToMQTT("off");
			Sensor->Aux =0;
		}
		

}

void confDevices(){
	switch(deviceType){
        case ONOFF_DEVICE:
			confOnOffDevice();
              break;
        case SHUTTER_DEVICE:
				confPersiana();
              break;
        case BUTTON_ONOFF_DEVICE:
				confButtonOnOff();
              break;
		case BUTTONS_UPDOWN_DEVICE:
				confButtonUpDown();
				break;
		case PHOTORESISTANCE_for_OnOff:
				confPhotoresistance();
          }
	
}
void confPersiana(){
  static int32_t		TiempoSubidaActual = std::atoi(leerEEPROM(ACTIVETIME_PERSIANA,ACTIVETIME_PERSIANAlenght).c_str());  //FIRST BOOT = 0
  ctrlPorPer.activa=false;
  ctrlPorPer.maxTime = (uint16_t *) &persianaTiempoSubida;
  ctrlPorPer.actTime		= &TiempoSubidaActual;
  percentage = std::atoi(leerEEPROM(PERCENTAGE_PERSIANA,PERCENTAGE_PERSIANAlength).c_str());

}

void confOnOffDevice(){
		Devices::setActuator(&devices.builtLed, leerEEPROMbyte(ON_OFF_DEVICE_STATUS)); 

}

void confButtonOnOff(){
	devices.BUTTON_1.SensorEvent =ctrlButtonOnOff;
	Serial.println("ENTRO EN TIPO BOTON");
}

void confButtonUpDown(){
	devices.BUTTON_1.Sensor.name="UP";
	devices.BUTTON_2.Sensor.name="DOWN";

	devices.BUTTON_1.SensorEvent =ctrlButtonUpDown;
	devices.BUTTON_2.SensorEvent =ctrlButtonUpDown;
	Serial.println("ENTRO EN TIPO BOTONES PERSIANA");
}

void confPhotoresistance(){
	static TctrlLuminosidad ctrlSRC;

	ctrlSRC.limiteInferior = std::atoi(leerEEPROM(PHOTORESISTANCE_LOWER_LIMIT,PHOTORESISTANCE_LOWER_LIMITlength).c_str());
	ctrlSRC.limiteSuperior = std::atoi(leerEEPROM(PHOTORESISTANCE_UPPER_LIMIT,PHOTORESISTANCE_UPPER_LIMITlength).c_str());
	
	//ctrlSRC.limiteInferior = 200;
	//ctrlSRC.limiteSuperior = 400;

	devices.PHOTORESISTANCE.Sensor.ctrlLuminosidadPtr=&ctrlSRC;
	devices.PHOTORESISTANCE.SensorEvent =ctrlPhotoresistance;

}



void splitAndSaveMqttPayload(const char* value){

    char* payload = (char*)value;
    char* delimiter=" ";
    char *name = NULL;
    char *res[3];
    int cont =0;
    name = strtok(payload, delimiter);
    while(name != NULL){
        res[cont]=name;
        cont++;
        name = strtok(NULL, delimiter); 
    }

	guardarEEPROM(MQTT_ROUTE_STATUS,res[0],MQTT_ROUTE_STATUSlength);
	guardarEEPROM(PHOTORESISTANCE_LOWER_LIMIT,res[1],PHOTORESISTANCE_LOWER_LIMITlength);
	guardarEEPROM(PHOTORESISTANCE_UPPER_LIMIT,res[2],PHOTORESISTANCE_UPPER_LIMITlength);
}

