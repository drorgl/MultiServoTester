#pragma once

uint8_t io_modes_size = 2;
enum class io_mode : uint8_t{
  PWM_IN = 0, 
  PWM_OUT = 1,
  //PPM_IN =2
};

const char * get_io_mode_string(enum io_mode mode){
    switch (mode){
        case io_mode::PWM_IN: return "PWMI";
        case io_mode::PWM_OUT: return "PWMO";
        //case io_mode::PPM_IN: return "PPMI";
        default:
        return "ERR";
    }
}