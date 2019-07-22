#pragma once 

#include <SoftRcPulseIn.h>

class InputController{
private:
    SoftRcPulseIn inputs[4];
    uint8_t input_pins[4] = {PIN_A5, PIN_A3, PIN_A7, PIN_B2};
    uint8_t size = 4;

    uint16_t values[4][4];
    uint8_t values_size[4] = {0,0,0,0};
public:
   void attach(uint8_t servoId){
        if (servoId > (size-1)){
            return;
        }
        pinMode(input_pins[servoId],INPUT);
        if (!inputs[servoId].attached()){
            inputs[servoId].attach(input_pins[servoId],500,2450);
        }
        inputs[servoId].enable();

        values_size[servoId] = 1;
    }

     void detach(uint8_t servoId){
        if (servoId > (size-1)){
            return;
        }

        inputs[servoId].disable();
        values_size[servoId] = 0;
    }

    uint16_t readValue(uint8_t servoId){
        if (servoId > (size-1)){
            return -1;
        }
        if (!inputs[servoId].available()){
            return 0;
        }

        return inputs[servoId].width_us();
    }

    uint16_t * get_values_array(uint8_t servoId){
        return values[servoId];
    }

    uint8_t get_values_size(uint8_t servoId){
        return values_size[servoId];
    }

    void loop(){
        for (uint8_t i = 0; i < size;i++){
            values[i][0] = readValue(i);
        }
    }
};