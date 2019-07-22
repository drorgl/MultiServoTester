#pragma once

#include "IOModes.h"

#include <TinyWireM.h>

#include <Tiny4kOLED.h>

#define OLED_WIDTH 128

#define BLINK_RATE 200

class Display
{
private:
    uint8_t max_io = 0;
    enum class io_mode *io_modes;
    int8_t current_io = -1;

    int current_value;

    unsigned long blink_time = 0;
    bool blink_state = false;

    uint8_t io_size = 4;
    uint16_t *io_values[4];
    uint8_t io_sizes[4] = {0, 0, 0, 0};

    bool isBlinkOn()
    {
        if (abs(millis() - blink_time) > BLINK_RATE)
        {
            blink_time = millis();
            blink_state = !blink_state;
        }
        return blink_state;
    }

public:
    Display()
    {
        oled.begin();
        oled.clear();
        oled.on();
        oled.switchRenderFrame();
    }

    void update_io(enum class io_mode *io_modes, uint8_t max_io, int8_t current_io)
    {
        this->max_io = max_io;

        this->io_modes = io_modes;
        this->current_io = current_io;
    }

    void update_inputs(uint8_t io, uint16_t *values, uint8_t values_size)
    {
        io_values[io] = values;
        io_sizes[io] = values_size;
    }

    void update_value(int value)
    {
        current_value = value;
    }

    void redraw()
    {
        auto showBlinking = isBlinkOn();
        oled.clear();

        auto state_width = OLED_WIDTH / io_size;

        oled.setFont(FONT6X8);
        if (max_io > 0)
        {

            for (uint8_t i = 0; i < max_io; i++)
            {
                oled.setCursor(i * state_width, 0);

                if (current_io == i)
                {
                    if (showBlinking)
                    {
                        oled.print(get_io_mode_string(io_modes[i]));
                    }
                }
                else
                {
                    oled.print(get_io_mode_string(io_modes[i]));
                }
            }
        }
        else
        {
            for (uint8_t i = 0; i < io_size; i++)
            {
                if (io_sizes[i] > 0)
                {
                    for (uint8_t l = 0; l < io_sizes[i]; l++)
                    {
                        oled.setCursor(i * state_width, l);
                        oled.print(io_values[i][l]);
                    }
                }
            }
        }

        oled.setCursor(90, 2);
        oled.setFont(FONT8X16DIGITS);
        oled.print(current_value);

        oled.switchFrame();
    }
};