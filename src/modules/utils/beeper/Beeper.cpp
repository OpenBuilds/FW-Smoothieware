
#include "Beeper.h"
#include "Kernel.h"
#include "Config.h"
#include "checksumm.h"
#include "ConfigValue.h"
#include "Gcode.h"
#include "libs/nuts_bolts.h"
#include "libs/utils.h"

#include "wait_api.h" // mbed

#define beeper_cs             CHECKSUM("beeper")
#define enable_cs             CHECKSUM("enable")
#define buzz_pin_checksum     CHECKSUM("buzz_pin")

/**
Parameters
Snnn frequency in Hz
Pnnn duration in milliseconds

Example
M300 S300 P1000
*/

/**
    example config...

    beeper.enable        true         #
    beeper.buzz_pin   1.22         #
*/

void Beeper::on_module_loaded()
{
    // if the module is disabled -> do nothing
    if(!THEKERNEL->config->value( beeper_cs, enable_cs )->by_default(false)->as_bool()) {
        // as this module is not needed free up the resource
        delete this;
        return;
    }

    this->buzz_pin.from_string(THEKERNEL->config->value( beeper_cs, buzz_pin_checksum)->by_default("nc")->as_string())->as_output();

    this->buzz(50, 300);
    // this->register_for_event(ON_IDLE);
    // this->register_for_event(ON_SECOND_TICK);
    this->register_for_event(ON_GCODE_RECEIVED);

}

void Beeper::buzz(long duration, uint16_t freq) {
    if(!this->buzz_pin.connected()) return;

    duration *=1000; //convert from ms to us
    long period = 1000000 / freq; // period in us
    long elapsed_time = 0;
    while (elapsed_time < duration) {
        this->buzz_pin.set(1);
        wait_us(period / 2);
        this->buzz_pin.set(0);
        wait_us(period / 2);
        elapsed_time += (period);
    }
}

void  Beeper::on_gcode_received(void *argument)
{
    Gcode *gcode = static_cast<Gcode *>(argument);

     if (gcode->has_m) {
        if (gcode->m == 300) {

            int s = 0;
            int p = 0;

            if(gcode->get_num_args() == 0) {
                // Don't beep
                return;
            }

            if(gcode->has_letter('P')){
                p= gcode->get_value('P');
                if (p < 1) p = 1;
            }

            if(gcode->has_letter('S')){
                s= gcode->get_value('S');
                if (s < 1) s = 1;
            }

            //  lcd->buzz(50, 300); // 50ms 300Hz
            this->buzz(p, s);

        }
    }

}
