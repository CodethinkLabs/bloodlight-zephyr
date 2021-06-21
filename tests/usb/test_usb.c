#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <devicetree.h>
#include <zephyr.h>
#include <sys/printk.h>
#include <sys/util.h>
#include <string.h>
#include <usb/usb_device.h>
#include <drivers/uart.h>
#include <ztest.h>
#include <drivers/usb/usb_dc.h>

#define SLEEP_TIME_MS   1000
#define LOOPS 10


void main (void) {
	const struct device *dev = device_get_binding(
			CONFIG_UART_CONSOLE_ON_DEV_NAME);
	uint32_t dtr = 0;

	if (usb_enable(NULL)) {
		return;
	}

	while (!dtr) {
		uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
	}

	if (strlen(CONFIG_UART_CONSOLE_ON_DEV_NAME) !=
		strlen("CDC_ACM_0") ||
	    strncmp(CONFIG_UART_CONSOLE_ON_DEV_NAME, "CDC_ACM_0",
		strlen(CONFIG_UART_CONSOLE_ON_DEV_NAME))) {
			printk("Error: Console device name is not USB ACM\n");

		return;
	}

	struct usb_ep_cfg_data data {

	};
	int ret;
	usb_dc_status_callback status;
	ret = usb_enable(status);
};