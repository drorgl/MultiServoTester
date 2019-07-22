# Multi Servo Tester

This is the firmware for the Multi Servo Tester project. 

The project is based on ATtiny84, an OLED display, a potentiometer and uses protection diodes on inputs and outputs.

## Input/Output
Current firmware supports setting pin (1-4) as either PWM In or PWM Out and all output pins are set as the current potentiometer position.
PWM In pins display their current value in the top part of the display.

## Starting up
MultiServoTester powers up when all pins are powered down.

Button 1 - selects the IO Pin
Button 2 - selects the pin mode, either PWM In or PWM Out
Button 3 - sets the pins as their designation.

To start all the pins as PWM Out, click the 3rd button after start, otherwise, select the pin modes and then click the 3rd button.

## micronucleus
while the board does support [micronucleus](https://github.com/micronucleus/micronucleus) USB bootloader, this firmware is larger than available space after the bootloader.

## Attribution
This project was made easier to implement by using the Digispark project.
https://github.com/RC-Navy/DigisparkArduinoIntegration
