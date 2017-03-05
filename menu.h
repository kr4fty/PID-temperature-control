#ifndef _MENU_H
#define _MENU_H

    void setTemp(uint8_t setTempFlag);

    uint16_t getValue(uint16_t value, uint8_t sCursorX, uint8_t sCursory, uint8_t sTextSize);

    void printTemp(uint16_t tempe);

    void printTemp(uint16_t tempe1, uint16_t tempe2);

    void printTempf(uint16_t tempe, uint8_t sCursorX, uint8_t sCursorY, uint8_t sTextSize);

    void setStatus(const char *status);   // Escribo notificacion en la ultima linea del display

    void clearStatus();                   // Limpio notificacion en la ultima linea del display

    void runProcess(bool run_status);     // Lo pauso o arranco segun el valor de run_status

    void runPauseProcess();               // Si se esta ejecutando lo pauso y viceversa

#endif
