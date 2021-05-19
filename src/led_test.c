#include <stddef.h>
#include <stdint.h>

#include "led.h"
#include "led.c"

void main (void) {

    bl_led_init();
    while(1){
        bl_led_loop();
    };
};