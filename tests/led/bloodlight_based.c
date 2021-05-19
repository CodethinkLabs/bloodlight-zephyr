#include <stddef.h>
#include <stdint.h>
#include <devicetree.h>
#include <zephyr.h>
#include <sys/printk.h>
#include <sys/util.h>
#include <string.h>
#include <usb/usb_device.h>
#include <drivers/uart.h>
#include <stdio.h>

void main (void) {
	const struct device *dev = device_get_binding(
			CONFIG_UART_CONSOLE_ON_DEV_NAME);
	uint32_t dtr = 0;


	if (usb_enable(NULL)) {
		return;
	}

#define SLEEP_TIME_MS   1000
/* The devicetree node identifier for the "led0" alias. */
#define STATUS_NODE DT_ALIAS(statusled)

#if DT_NODE_HAS_STATUS(STATUS_NODE, okay)
#define STATUS_LED    DT_GPIO_LABEL(STATUS_NODE, gpios)
#define STATUS_PIN     DT_GPIO_PIN(STATUS_NODE, gpios)
#define STATUS_FLAGS   DT_GPIO_FLAGS(STATUS_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define STATUS_LED    ""
#define STATUS_PIN     0
#define STATUS_FLAGS   0
#endif

void main (void) {
	while (1) {
		bl_led_status_set(true);
		k_msleep(SLEEP_TIME_MS);
		bl_led_status_set(false);
		k_msleep(SLEEP_TIME_MS);
	}
};