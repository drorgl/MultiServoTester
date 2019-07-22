#include <Arduino.h>

#include "IOModes.h"

#include "Display.h"
#include "ServoController.h"
#include "ResistorLadderButton.h"
#include "InputController.h "

#define POT_PIN A1

ResistorLadderButton buttons;
Display display;
ServoController servos;
InputController inputs;

const uint8_t max_io = 4;
enum io_mode io_modes[4] = {io_mode::PWM_OUT, io_mode::PWM_OUT, io_mode::PWM_OUT, io_mode::PWM_OUT};
enum io_mode current_io_modes[4] = {io_mode::PWM_OUT, io_mode::PWM_OUT, io_mode::PWM_OUT, io_mode::PWM_OUT};
int8_t current_io = -1;

void update_io_handlers()
{
    //current_io_modes,max_io
    for (uint8_t i = 0; i < max_io; i++)
    {
        servos.detach(i);
        inputs.detach(i);
        display.update_inputs(i, NULL, 0);

        switch (current_io_modes[i])
        {
        case io_mode::PWM_OUT:
            servos.attach(i);
            break;
        case io_mode::PWM_IN:
            inputs.attach(i);
            display.update_inputs(i, inputs.get_values_array(i), inputs.get_values_size(i));
            break;
        }
    }
}

//set selected io
void button_0()
{
    if (current_io < (max_io - 1))
    {
        current_io++;
    }
    else
    {
        current_io = -1;
    }
    display.update_io((enum io_mode *)&io_modes, max_io, current_io);
}

//move the io mode
void button_1()
{
    if (current_io < max_io && current_io != -1)
    {
        io_modes[current_io] = (io_mode)(((uint8_t)io_modes[current_io]) + 1);
        if ((uint8_t)io_modes[current_io] > (io_modes_size - 1))
        {
            io_modes[current_io] = (io_mode)0;
        }
    }
    display.update_io((enum io_mode *)&io_modes, max_io, current_io);
}

//save the mode
void button_2()
{
    if (current_io < max_io && current_io != -1)
    {
        for (uint8_t i = 0; i < max_io; i++)
        {
            current_io_modes[i] = io_modes[i];
        }
        current_io = -1;
    }

    display.update_io((enum io_mode *)&current_io_modes, 0, current_io);
    update_io_handlers();
}

// cppcheck-suppress unusedFunction
void setup()
{
    pinMode(POT_PIN, INPUT);
    buttons.attachCallback(0, button_0);
    buttons.attachCallback(1, button_1);
    buttons.attachCallback(2, button_2);
    display.update_io((enum io_mode *)&current_io_modes, max_io, current_io);
}

void loop()
{
    buttons.loop();
    inputs.loop();
    auto val = analogRead(POT_PIN);
    uint16_t val_micros = map(val, 0, 1023, 544, 2400);
    display.update_value(val_micros);
    for (uint8_t i = 0; i < 4; i++)
    {
        servos.writeMicroseconds(i, val_micros);
    }

    display.redraw();
}
