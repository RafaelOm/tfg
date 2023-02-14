#include "EEPROM_Utils.h"

void guardarEEPROM(int addr, String dato,int EEPROMSIZE){

    int size = dato.length(); 
    char inchar[EEPROMSIZE]; 
    dato.toCharArray(inchar, size+1);

    for (int i = 0; i < size; i++) {
        EEPROM.write(addr+i, (uint8_t) inchar[i]);
    }
    for (int i = size; i < EEPROMSIZE; i++) {
        EEPROM.write(addr+i, (uint8_t) 255);//Rellenamos los huecos que quedan libres de la EEPROM
    }
    
    EEPROM.commit();

}


String leerEEPROM(int addr,int EEPROMSIZE){
    byte lectura;
    String strlectura;
    for (int i = addr; i < addr+EEPROMSIZE; i++) {
        lectura = EEPROM.read(i);
        if (lectura != 255) {
            strlectura += (char)lectura;
        }
   }
   return strlectura;

}

void guardarEEPROMbyte(int addr, int dato){
    EEPROM.write(addr, (uint8_t) dato);
    EEPROM.commit();

}
int leerEEPROMbyte(int addr){

    return (int) EEPROM.read(addr);
}