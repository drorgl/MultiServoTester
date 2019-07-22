#pragma once

#include <Arduino.h>
#include <Servo.h>

class ServoController
{
private:
    Servo servo[4];
    uint8_t servo_pins[4] = {PIN_A5, PIN_A3, PIN_A7, PIN_B2};
    uint8_t size = 4;

public:
    ServoController()
    {
    }

    void attach(uint8_t servoId)
    {
        if (servoId > (size - 1))
        {
            return;
        }
        pinMode(servo_pins[servoId], OUTPUT);
        servo[servoId].attach(servo_pins[servoId]);
    }

    void detach(uint8_t servoId)
    {
        if (servoId > (size - 1))
        {
            return;
        }
        servo[servoId].detach();
        pinMode(servo_pins[servoId], INPUT);
    }

    void writeMicroseconds(uint8_t servoId, uint16_t ms)
    {
        if (servoId > (size - 1))
        {
            return;
        }
        servo[servoId].writeMicroseconds(ms);
    }
};