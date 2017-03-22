# PID-temperature-control

Oven/heater temperature control made with an Arduino Uno (atmega328p microcontroller).
For the same it is used a PID controller which will provide to the output a constant temperature of acuardo to the set by the user.

This is an implementation of the ovenControl library for Arduino boards

## Description:
The system maintains a constant temperature at which it is selected. It can vary between 0 and 420ºC.

## Software
These are the libraries required for their operation:

* Lcd: [Adafruit-PCD8544-Nokia-5110-LCD-library](https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library).
* Keyboard: [AnalogKeypad](https://github.com/kr4fty/AnalogKeyPad).
* Menu: [MenuPCD8544](https://github.com/kr4fty/MenuPCD8544).
* K thermocouple: [K-Type_thermocouple-Library](https://github.com/kr4fty/K-Type_thermocouple-Library).
* OvenControl: [ovenControl](https://github.com/kr4fty/ovenControl).


## Hardware:
The system is composed of five main parts

* **The microcontroller**: brain in charge of the administration of resources.
* **Display and keyboard**: is the user interface. It uses a lcd of a Nokia 3310/5510 and a ladder type keyboard.
* **The temperature sensor**: in charge of constantly measuring the temperature of the oven/heater and in this way can make the feedback towards the control for the respective adjustments. For this function a K-type thermocouple  is used.
* **AC control**: in charge of handling AC, in this case I will use 220Vac/50Hz. For this purpose opto-thyristors are used, which act as a solid state relay.
* **Zero crossing detector**: in charge of activating the thyristors from the moment in which the alternating voltage wave is detected crosses by zero.For this function a K-type thermocouple is used.

![alt tag](https://github.com/kr4fty/PID-temperature-control/blob/master/Eagle%20PCB/circuit.png)

## Video
[![Oven Control](https://img.youtube.com/vi/Dz5Lqh4SREI/0.jpg)](https://www.youtube.com/watch?v=Dz5Lqh4SREI "PID Oven Control")
