/*
 *  Nokia 3310/5110, 84x48 pixel monochrome LCD display.
 */
 
#include <MenuPCD8544.h>
#include <AnalogKeyPad.h>
#include <PID_v1.h>
#include <oven.h>
#include <thermo.h>
#include "menu.h"
#include "parameters.h"

extern const menu_t menuRoot;

extern Adafruit_PCD8544 lcd;
extern AnalogKeyPad keypad;
extern MenuPCD8544 menuMain;

double temp= 5, tempAnt;
extern uint16_t tempLimit1;
extern bool showMenuFlag;
extern uint8_t backLightMode;
extern uint32_t autoOffValue;
extern bool alarmEnabled;
extern unsigned long counter;
extern bool isBacklightOn;

extern OvenControl calentador;
extern double setPoint;

extern bool itsRun;
extern bool itsStop;
extern bool waitAHitToUpdate;

void printPrincipal();

unsigned long nextCheck=millis();

void setup()
{
  lcd.begin();
  lcd.setRotation(2);
  lcd.cp437(true);
  pinMode(BACKLIGHTPIN, OUTPUT);
  pinMode(ALARMINDICATORPIN, OUTPUT);
  analogReference(DEFAULT);

  backLightMode = 0;
  digitalWrite(BACKLIGHTPIN, LOW);

  //PID
  setPoint=100;
  calentador.setAccuracy(2);
  calentador.setPWMSkip(0);
  calentador.myPID->SetTunings(KP, KI, KD);
  calentador.moveTo(setPoint);
  //FIN PID

  printPrincipal();

  tone(ALARMSOUNDPIN, 820, 300);
  delay(100);
  tone(ALARMSOUNDPIN, 260, 200);
  delay(100);
  tone(ALARMSOUNDPIN, 440, 300);
  delay(100);
  noTone(ALARMSOUNDPIN);

}

void loop()
{
  menuFunc_t selection;
  int8_t keyCode;

  tempAnt = temp;
  temp = calentador.getActualTemperature();

  //PID
  if (millis() > nextCheck && itsRun && !itsStop)
  {
    nextCheck += 50;
    //Serial.println(temp);

    calentador.run();

    if(!calentador.finished())
    {
      //Serial.println("Controlando temp");
      calentador.moveTo(setPoint);
    }
    else
    {
      //Serial.println("Dentrol del rango");
      calentador.stop();
    }
  }//FIN PID

  if( alarmEnabled )
  {
    if( (tempLimit1-5)<=temp )
    {
      digitalWrite(ALARMINDICATORPIN, HIGH);

      tone(ALARMSOUNDPIN, 2000, 50);
      delay(150);
      tone(ALARMSOUNDPIN, 2000, 100);
    }
    else
    {
      digitalWrite(ALARMINDICATORPIN, LOW);
      noTone(ALARMSOUNDPIN);
    }
  }

  if( (temp != tempAnt) && (!showMenuFlag) && (!waitAHitToUpdate))
  {
    printTemp(temp);
    lcd.display();
  }

  if( (!isBacklightOn) && (backLightMode==2) )  // Si esta apagado, modo auto y mostrando temps
  {
    if( keypad.keyUpEvent() != AnalogKeyPad::NO_KEY )
    {
      counter = millis();
      digitalWrite(BACKLIGHTPIN, HIGH);
      isBacklightOn = true;
    }
  }
  else
  {
    if( !waitAHitToUpdate )
    {
      if( !showMenuFlag )
      {
        if( keypad.keyUpEvent() != AnalogKeyPad::NO_KEY )
        {
          if(backLightMode==2)
            counter = millis();
          if( (keypad.getCurrentKey() == AnalogKeyPad::UP ) && !itsStop )
          {
            runPauseProcess();
            printPrincipal();
          }
          if( keypad.getCurrentKey() == AnalogKeyPad::SELECT )
          {
            tone(ALARMSOUNDPIN, 200, 200);
            delay(100);
            tone(ALARMSOUNDPIN, 440, 300);
            delay(100);
            noTone(ALARMSOUNDPIN);

            lcd.clearDisplay();

            showMenuFlag = true;
            menuMain.begin(&lcd);
          }
        }
      }
      else
      {
        selection = taskMenu();             // run the menu task
        if ( selection != NULL )            // menu item selected?
        {
          showMenuFlag = false;
          (*selection)();                   // then execute it

          if( !waitAHitToUpdate )
          {
            printPrincipal();
          }

          tone(ALARMSOUNDPIN, 480, 300);
          delay(100);
          tone(ALARMSOUNDPIN, 200, 200);
          delay(100);
          noTone(ALARMSOUNDPIN);
        }
      }
    }
    else
      if( keypad.keyUpEvent() != AnalogKeyPad::NO_KEY )
      {
        waitAHitToUpdate = false;
        printPrincipal();
      }
  }

  if( (backLightMode==2) &&  ((millis()-counter) > autoOffValue) )
  {
    digitalWrite(BACKLIGHTPIN, LOW);
    isBacklightOn = false;
  }
}


// handle the menu
menuFunc_t taskMenu()
{
	menuFunc_t doit = NULL;
	int8_t keyCode;

  if ( (keyCode = keypad.keyUpEvent()) != AnalogKeyPad::NO_KEY )       // was key pressed?
  {
    if( (!isBacklightOn) && (backLightMode==2) )
    {
      digitalWrite(BACKLIGHTPIN, HIGH);
      isBacklightOn = true;
    }
    counter = millis();

    tone(ALARMSOUNDPIN, 440, 50);
    delay(50);
    noTone(ALARMSOUNDPIN);

    // valid input keycodes: U,D,L,R,S - Up, Down, Left, Right, Select
    // returns true if menu item was selected and *doit holds the associated function pointer
    int8_t selected = menuMain.navigate(keyCode, &doit);

    if (selected)               		        // user selected the menu item
    {
      if (doit == MenuPCD8544::canceled)      // check that the menu was not canceled
      {   doit = NULL;

        printPrincipal();

        showMenuFlag = false;
      }
    }
  } // end process keycode
  return doit;
}

void printPrincipal()
{
    lcd.clearDisplay();
    lcd.setTextSize(1);
    lcd.setTextColor(BLACK, WHITE);

    if( !itsStop )
    {
      lcd.print("Setpoint= ");
      lcd.print((uint16_t)setPoint);

      if( !itsRun )
        setStatus("   Paused");
      else
        setStatus("  Running...");
    }

    else
    {
      lcd.print("Temperatura");
      setStatus("  OK -> Menu");
    }
    lcd.setTextSize(2);
    lcd.setCursor(53, 14);
    lcd.print((char)248);
    lcd.print("C");

    temp --;
}
