#ifndef _MENU_H
#define _MENU_H

    void setTemp(uint8_t setTempFlag);

    uint16_t getValue(uint16_t value, uint8_t sCursorX, uint8_t sCursory, uint8_t sTextSize);

    void printTemp(uint16_t tempe);

    void printTemp(uint16_t tempe1, uint16_t tempe2);

    void printTempf(uint16_t tempe, uint8_t sCursorX, uint8_t sCursorY, uint8_t sTextSize);

    void setStatus(const char *status);

    void clearStatus();
    
    void runPauseProcess();

#endif
