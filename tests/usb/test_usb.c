#include <stdint.h>
#include <stdio.h>
#include <zephyr.h>
#include <string.h>

#include <logging/log.h>
#include <drivers/uart.h>
#include <usb/usb_device.h>
#include <sys/ring_buffer.h>

#include "../../src/usb.h"

#define SLEEP_TIME_MS   1000
#define LOOPS 10

void main (void) {
	bl_usb_init();
	
}