#pragma once

#include "Module.h"
#include "Pin.h"
#include "Pwm.h"

#include <stdint.h>
#include <vector>

class Beeper : public Module
{
public:
    ~Beeper(){};

    void on_module_loaded();
    void buzz(long duration, uint16_t freq);
    void on_gcode_received(void *argument);

private:
    Pin buzz_pin;
};
