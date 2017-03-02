//#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <AnalogKeyPad.h>
#include <MenuPCD8544.h>
#include <thermo.h>
#include <oven.h>
#include "menu.h"
#include "parameters.h"

// Submenu BackLight
static const int8_t NUM_ITEMS_SUBMENU_BACKLIGHT = 4;
const char menuBacklightItem1[] PROGMEM = "BLight ON";
const char menuBacklightItem2[] PROGMEM = "AutoOff ON";
const char menuBacklightItem3[] PROGMEM = "BLight OFF";
const char menuBacklightItem4[] PROGMEM = "Time AutoOff";
void backLightOnFun();
void backlightOffFun();
void automaticFun();
void setAutoOffFun();
const menuItem_t menuBacklightItems[NUM_ITEMS_SUBMENU_BACKLIGHT] PROGMEM =
{
  {menuBacklightItem1, &backLightOnFun},
  {menuBacklightItem2, &automaticFun},
  {menuBacklightItem3, &backlightOffFun},
  {menuBacklightItem4, &setAutoOffFun}
};
const menu_t submenuBacklight PROGMEM =
{ NUM_ITEMS_SUBMENU_BACKLIGHT, menuBacklightItems };

// Submenu Ver
static const int8_t NUM_ITEMS_SUBMENU_VER = 3;
const char menuVerItem1[] PROGMEM = "Ver T1-T2";
const char menuVerItem2[] PROGMEM = "Solo T1";
const char menuVerItem3[] PROGMEM = "Solo T2";
void t1Yt2Fun();
void soloT1Fun();
void soloT2Fun();
const menuItem_t menuVerItems[NUM_ITEMS_SUBMENU_VER] PROGMEM =
{
  {menuVerItem1, &t1Yt2Fun},
  {menuVerItem2, &soloT1Fun},
  {menuVerItem3, &soloT2Fun}
};
const menu_t submenuVer PROGMEM =
{ NUM_ITEMS_SUBMENU_VER, menuVerItems};

// Submenu Cnfigurar SetPoint
static const int8_t NUM_ITEMS_SUBMENU_SETPOINT = 3;
const char menuSetpointItem1[] PROGMEM = "Set valor";
const char menuSetpointItem2[] PROGMEM = "Run/Pause";
const char menuSetpointItem3[] PROGMEM = "Cancelar";
void setSetpoint();
//void runPauseProcess();
void cancelProcess();
const menuItem_t menuSetpointItems[NUM_ITEMS_SUBMENU_SETPOINT] PROGMEM =
{
    {menuSetpointItem1, &setSetpoint},
    {menuSetpointItem2, &runPauseProcess},
    {menuSetpointItem3, &cancelProcess}
};
const menu_t submenuSetpoint PROGMEM=
{NUM_ITEMS_SUBMENU_SETPOINT, menuSetpointItems};

// Submenu Alarmas
static const int8_t NUM_ITEMS_SUBMENU_ALARMAS = 3;
const char menuAlarmasItem1[] PROGMEM = "Estado";
const char menuAlarmasItem2[] PROGMEM = "E/D alarm";
const char menuAlarmasItem3[] PROGMEM = "Set valor";
void valoresTempFun();
void enableAlarmFun();
void setT1Fun();
const menuItem_t menuAlarmasItems[NUM_ITEMS_SUBMENU_ALARMAS] PROGMEM =
{
  {menuAlarmasItem1, &valoresTempFun},
  {menuAlarmasItem2, &enableAlarmFun},
  {menuAlarmasItem3, &setT1Fun}
};
const menu_t submenuAlarmas PROGMEM =
{ NUM_ITEMS_SUBMENU_ALARMAS, menuAlarmasItems};

/*******************************************************************
 * Main menu definition
 *******************************************************************/

// maximum number of menu items in a menu is 5
// if you want more you need to split your menu into submenus
const int8_t NUM_MAIN_MENU_ITEMS = 5;

// menu entries shown to the user, they are located in the program memory
const char menuMainItem1[] PROGMEM = ":SetPoint";
const char menuMainItem2[] PROGMEM = ":Alarmas";
const char menuMainItem3[] PROGMEM = ":BackLight";
const char menuMainItem4[] PROGMEM = "Calibrar";
const char menuMainItem5[] PROGMEM = "Acerca de..."; // the ':' indicates a submenu, it is not displayed as part of the menu item


// functions corresponding to the menu entries
void calibrarFun();
void acercaDeFun();

// menu definition: a menu is an array of tuples: (menu text, menu function)
const menuItem_t menuMainItems[NUM_MAIN_MENU_ITEMS] PROGMEM =
{
    { menuMainItem1, (menuFunc_t) &submenuSetpoint},
    { menuMainItem2, (menuFunc_t) &submenuAlarmas},
    { menuMainItem3, (menuFunc_t) &submenuBacklight},
    { menuMainItem4, &calibrarFun},
    { menuMainItem5, &acercaDeFun } // submenus must be cast to menuFunc_t

};

// top level menu node: it consist of the number of items in the menu
//                      and a pointer to the array of menu items 
const menu_t menuRoot PROGMEM =
{ NUM_MAIN_MENU_ITEMS, menuMainItems };

/******************************************************************************
 *******************************Fin definition de Menu*************************
 *****************************************************************************/

MenuPCD8544 menuMain(&menuRoot);
Adafruit_PCD8544 lcd = Adafruit_PCD8544(DISPLAYSCKPIN, DISPLAYDINPIN, DISPLAYDCPIN, DISPLAYCSPIN, DISPLAYRSTPIN);
AnalogKeyPad keypad(ANALOGKEYPADPIN);

uint16_t tempLimit1, tempLimit2;
bool showMenuFlag = false;
uint8_t backLightMode = 1;
uint32_t autoOffValue = 60000;
uint8_t windowFlag = 0;
bool alarmEnabled = false;
unsigned long counter;
bool isBacklightOn = false;

double setPoint;
double Kp=KP, Ki=KI, Kd=KD;
OvenControl calentador = OvenControl(THERMOCOUPLEPIN, ZEROCROSSPIN, PWMOUTPUTPIN);

bool itsRun= false;
bool itsCancel= true;

void backLightOnFun()
{
    digitalWrite(BACKLIGHTPIN, 1);
    backLightMode = 1;
    //setStatus("Back Light ON");
}
void backlightOffFun()
{
    digitalWrite(BACKLIGHTPIN, 0);
    backLightMode = 0;
    //setStatus("BackLight OFF");
}
void automaticFun()
{
    digitalWrite(BACKLIGHTPIN, HIGH);
    isBacklightOn = true;
    backLightMode = 2;
    counter= millis();
    //setStatus("AutoOff is ON");
}
void setAutoOffFun()
{
    uint16_t autoAux = autoOffValue/1000;
    
    lcd.clearDisplay();
    lcd.drawRect(1, 5, 83, 33,BLACK);
    lcd.setCursor(64, 18);
    lcd.print("SEG");
    setStatus("Setting AuOff");
    
    autoAux = getValue(autoAux, 5, 12, 3);
    autoOffValue = autoAux*1000;
    
    automaticFun();
}
void t1Yt2Fun()
{
    windowFlag = 0;
    setStatus("Ver T1 y T2");
}
void soloT1Fun()
{
    windowFlag = 1;
    setStatus("Ver SOLO T1");
}
void soloT2Fun()
{
    windowFlag = 2;
    setStatus("Ver SOLO T2");
}
void valoresTempFun()
{
    int8_t keyCode;
    lcd.clearDisplay();
    lcd.setCursor(0, 2);
    lcd.print("Alarma:");
    lcd.setTextSize(2);
    lcd.setCursor(59, 10);
    lcd.print((char)248);
    lcd.print("C");    
    
    printTempf(tempLimit1, 5, 12, 3);
    
    alarmEnabled?setStatus("Alarm is ON"):setStatus("Alarm is OFF");
    lcd.display();
    
    while( (keyCode = keypad.keyUpEvent()) == AnalogKeyPad::NO_KEY );
    
    lcd.setTextSize(1); 
    lcd.clearDisplay();
}
void setSetpoint()
{
    lcd.clearDisplay();
    
    setStatus("Set SetPoint");
    
    lcd.drawRect(1, 5, 83, 33,BLACK);
    lcd.setCursor(64, 18);                
    lcd.print((char)248);
    lcd.setTextSize(2); 
    lcd.print("C");
    
    setPoint= (uint16_t) getValue(setPoint, 5, 12, 3);
    
    lcd.display();
    
    itsRun = true;
    itsCancel = false;
}
void runPauseProcess()
{
    if(itsRun){
        itsRun = false;
        calentador.setTurnOnHeater(false);
    }
    else{
        itsRun = true;
        calentador.setTurnOnHeater(true);
    }
    itsCancel = false;
}
void cancelProcess()
{
    itsCancel = true;
    itsRun = false;
    calentador.setTurnOnHeater(false);
}
void enableAlarmFun()
{
    lcd.clearDisplay();
    lcd.setCursor(14, 9);
    lcd.setTextSize(2);
    lcd.print("Alarm");
    lcd.setCursor(10, 25);
    switch(alarmEnabled)
    {
        case true:
                alarmEnabled = true;
                lcd.print("is ON");
                break;
        case false:
                alarmEnabled = false;
                lcd.print("is OFF");
                break;
    }
    lcd.display();
    while( keypad.keyUpEvent() == AnalogKeyPad::NO_KEY );
    lcd.clearDisplay();
    lcd.setTextSize(1);
}
void setAmbasTempFun()
{
    alarmEnabled = true;
    setTemp(1);
    tempLimit2=tempLimit1;
    setTemp(2);
    valoresTempFun();
    //setStatus("SET T1 y T2");
}
void setT1Fun()
{
    alarmEnabled = true;
    setTemp(1);
    valoresTempFun();
    //setStatus("SET T1");
}
void setT2Fun()
{
    alarmEnabled = true;
    setTemp(2);
    valoresTempFun();
    //setStatus("SET T2");
}
void calibrarFun()
{
    int8_t keyCode;
    lcd.clearDisplay();
    lcd.print("Ingrese la termocupla en un recipiente a 0");
    lcd.print((char)248);
    lcd.print("C y presione ");
    lcd.setTextColor(WHITE, BLACK);
    lcd.print("OK");
    lcd.setTextColor(BLACK, WHITE);
    lcd.print(" para finalizar.");
    lcd.display();
    while( ((keyCode = keypad.keyUpEvent()) != AnalogKeyPad::SELECT) && (keyCode!= AnalogKeyPad::LEFT) );
    if( keyCode==AnalogKeyPad::SELECT)
    {
        //myTC.calibrate = 0;
        calentador.setCalibrateTC((uint8_t)calentador.getActualTemperature());
        lcd.clearDisplay();
        lcd.println(calentador.getCalibrateTC());
        lcd.print("\n  Calibracion\n       OK!");
    }
    else if( keyCode==AnalogKeyPad::LEFT )
    {
        lcd.clearDisplay();
        lcd.print("\n\n Calibracion\n  Cancelada");
    }
    lcd.display();
    while( (keyCode=keypad.keyUpEvent()) == AnalogKeyPad::NO_KEY );
}
void acercaDeFun()
{
    lcd.clearDisplay();
    lcd.fillRect(5, 5, 74, 38,BLACK);
    lcd.setTextSize(2);
    lcd.setTextColor(WHITE, BLACK);
    lcd.setCursor(12, 18);
    lcd.print("Favio");
    lcd.display();
    while( keypad.keyUpEvent() == AnalogKeyPad::NO_KEY );
    lcd.clearDisplay();
    lcd.setTextSize(1);
    lcd.setTextColor(BLACK, WHITE);
    //setStatus("AUTOR");
}
void setTemp(uint8_t setTempFlag)
{
    uint16_t tempAux;
    
    lcd.clearDisplay();
    switch(setTempFlag)
    {
        case 1:
                setStatus("Setting T1");
                tempAux = (uint16_t)tempLimit1;
                break;
        case 2:
                setStatus("Setting T2");
                tempAux = (uint16_t)tempLimit2;
                break;
    }
        
    lcd.drawRect(1, 5, 83, 33,BLACK);
    lcd.setCursor(64, 18);                
    lcd.print((char)248);
    lcd.setTextSize(2); 
    lcd.print("C");
    
    tempAux = (uint16_t) getValue(tempAux, 5, 12, 3);
    
    switch(setTempFlag)
    {
        case 1:
                tempLimit1 = tempAux;
                break;
        case 2:
                tempLimit2 = tempAux;
                break;
    }
    
    lcd.display();
}
uint16_t getValue(uint16_t value, uint8_t sCursorX, uint8_t sCursory, uint8_t sTextSize)
{
    uint8_t keyCode;
    uint8_t num[3];
    uint8_t posX = 0;
    
    num[0] = (uint8_t)(value/100);
    num[1] = (uint8_t)((value%100)/10);
    num[2] = (uint8_t)(value%10);
    
    lcd.setTextSize(sTextSize);
    while( keyCode != AnalogKeyPad::SELECT )
    {
        lcd.setCursor(sCursorX, sCursory);
        for(int i=0; i<3; i++)
        {
            if( i==posX )
                lcd.setTextColor(WHITE, BLACK);
            else
                lcd.setTextColor(BLACK, WHITE);
            lcd.print(num[i]);
        }
        lcd.display();
        while( (keyCode = keypad.keyUpEvent()) == AnalogKeyPad::NO_KEY );
        switch(keyCode)
        {
            case 'L':
                if( (posX-1)>=0)
                    posX--;
                else
                    posX=2;
                break;
            case 'R':
                if( (posX+1)<=2)
                    posX++;
                else
                    posX=0;
                break;
            case 'D':
                if( (num[posX]-1) >= 0 )
                    num[posX]--;
                else
                    num[posX]=9;
                break;
            case 'U':
                if( (num[posX]+1) <= 9 )
                    num[posX]++;
                else
                    num[posX]=0;
                break;
            case '-':
                break;
        }
    }

    lcd.setTextSize(1);
    lcd.setTextColor(BLACK, WHITE);
    lcd.clearDisplay();
    
    value = (uint16_t)(100*num[0]+10*num[1]+num[2]);
    return value;
}

void printTemp(uint16_t tempe)
{
    printTempf(tempe, 0, 14, 3);
}
void printTemp(uint16_t tempe1, uint16_t tempe2)
{
    printTempf(tempe1, 18, 5, 2);
    printTempf(tempe2, 18, 23, 2);
}

void printTempf(uint16_t tempe, uint8_t sCursorX, uint8_t sCursorY, uint8_t sTextSize)
{
    lcd.setTextSize(sTextSize);
    lcd.setCursor(sCursorX, sCursorY);
    if(tempe<10)
    {
        lcd.print("  ");
    }
    else if(tempe>=10 && tempe<100)
    {
        lcd.print(" ");
    }
    lcd.print(tempe);
    lcd.setTextSize(1);
    //lcd.setCursor(0, 0);
}

void setStatus(const char *status)
{
  lcd.setTextSize(1);
  lcd.setCursor(0,40);
  lcd.setTextColor(WHITE, BLACK);
  lcd.print(status);
  for(int i=0; i<13-strlen(status); i++)
    lcd.print(" ");
  lcd.display();
  lcd.setTextColor(BLACK, WHITE);
}
void clearStatus()
{
  lcd.setTextSize(1);
  lcd.setCursor(0,40);
  lcd.print("             ");
  lcd.display();
}
