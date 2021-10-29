#include <stdint.h>
#include <stdio.h>
#include <zephyr.h>
#include <string.h>

#include <logging/log.h>
#include <drivers/uart.h>
#include <usb/usb_device.h>
#include <sys/ring_buffer.h>

#include "../../src/usb.h"
#include "../../src/bl.h"

#define SLEEP_TIME_MS   1000

#include "../../common/led.h"
#include "../../src/led.h"
void main (void) {
	bl_usb_init();
	//bl_led_status_set(true);
	//#k_msleep(300);
	//bl_led_status_set(false);
	//k_msleep(300);
	//__ASSERT(bl_init == true, "bl_usb_init() failed on initializing usb", false);
	//bl_led_status_set(true);
	bl_init();
	printf("SUCCESS\n");
}