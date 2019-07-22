#pragma once

#include <Arduino.h>

#define BUTTONS_PIN A2

class ResistorLadderButton{
    unsigned long press_time[3] = {0,0,0};

    void (*callbacks[3])(void);

    const int values[3] = {90,45,25};
    const int size = 3;
    const int hysteresis = 10;

    bool buttons[3] = {false,false,false};
public:
    ResistorLadderButton(){
        pinMode(BUTTONS_PIN,INPUT);
    }

    void attachCallback(uint8_t buttonId, void (*callback)()){
        callbacks[buttonId] = callback;
    }

    int readButton(){
        return analogRead(BUTTONS_PIN);
    }

    void loop(){
        auto val = readButton();

        for (uint8_t i = 0; i < size;i++){
            if (abs(val - values[i]) < hysteresis){
                if (buttons[i] == false){
                    buttons[i] = true;
                    if (press_time[i] == 0){
                        press_time[i] = millis();
                    }
                }
            }else{
                if (buttons[i] == true){
                    buttons[i] = false;
                    if (press_time[i] != 0){
                        callbacks[i]();
                        press_time[i] = 0;
                    }
                }
            }
        }
    }

    int isButtonPressed(uint8_t i ){
        return (buttons[i]) ? millis() - press_time[i] : 0;
    }
};