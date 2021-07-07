#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <zephyr.h>
#include <string.h>
#include <devicetree.h>
#include <sys/util.h>
#include <sys/printk.h>
#include <sys/__assert.h>
#include <drivers/uart.h>
#include <drivers/gpio.h>
#include <usb/usb_device.h>
#include "../../common/led.h"
#include "../../src/led.h"

#define SLEEP_TIME_MS   1000
#define LOOPS 10
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

	printf("\n*****Starting LED APIs tests****\n");

	printf("\nTesting bl_led_get_port\n");
	for (int led = 0; led < BL_LED_COUNT; led++){
		switch (led)
		{
		#if (BL_REVISION == 1)
			case 4:
			case 5:
			case 6:
			case 7:
				__ASSERT(((GPIO_TypeDef *) GPIOA_BASE) == bl_led_get_port(led), "Invalid port value in led %i\n", led);
				break;
			case 0:
			case 1:
			case 2:
			case 3:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
				__ASSERT(((GPIO_TypeDef *) GPIOB_BASE) == bl_led_get_port(led), "Invalid port value in led %i\n", led);
				break;
			case 8:
			case 9:
			case 10:
				__ASSERT(((GPIO_TypeDef *) GPIOC_BASE) == bl_led_get_port(led), "Invalid port value in led %i\n", led);
				break;
		#else
			case 1:
			case 4:
			case 5:
			case 6:
			case 7:
			case 11:
			case 13:
			case 14:
			case 15:
				__ASSERT(((GPIO_TypeDef *) GPIOA_BASE) == bl_led_get_port(led), "Invalid port value in led %i\n", led);
					break;
			case 2:
			case 9:
			case 10:
			case 12:
				__ASSERT(((GPIO_TypeDef *) GPIOB_BASE) == bl_led_get_port(led), "Invalid port value in led %i\n", led);
				break;
			case 0:
			case 3:
			case 8:
				__ASSERT(((GPIO_TypeDef *) GPIOC_BASE) == bl_led_get_port(led), "Invalid port value in led %i\n", led);
				break;
		#endif
		}
	}
		printf("\nSuccess testing bl_led_get_port\n");


	printf("\nTesting bl_led_get_gpio\n");
	for (int led = 0; led < BL_LED_COUNT; led++){
		switch (led)
		{
		#if (BL_REVISION == 1)
			case 0:
				__ASSERT((1U << 1) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 1:
				__ASSERT((1U << 2) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 2:
				__ASSERT((1U << 10) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 3:
				__ASSERT((1U << 11) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 4:
				__ASSERT((1U << 8) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 5:
				__ASSERT((1U << 9) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 6:
				__ASSERT((1U << 10) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 7:
				__ASSERT((1U << 15) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 8:
				__ASSERT((1U << 13) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 9:
				__ASSERT((1U << 14) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 10:
				__ASSERT((1U << 15) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 11:
				__ASSERT((1U << 9) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 12:
				__ASSERT((1U << 8) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 13:
				__ASSERT((1U << 7) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 14:
				__ASSERT((1U << 6) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 15:
				__ASSERT((1U << 5) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
		#else
			case 0:
				__ASSERT((1U << 14) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 1:
				__ASSERT((1U << 10) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 2:
				__ASSERT((1U << 11) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 3:
				__ASSERT((1U << 13) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 4:
				__ASSERT((1U << 6) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 5:
				__ASSERT((1U << 8) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 6:
				__ASSERT((1U << 9) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 7:
				__ASSERT((1U << 5) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 8:
				__ASSERT((1U << 15) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 9:
				__ASSERT((1U << 6) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 10:
				__ASSERT((1U << 5) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 11:
				__ASSERT((1U << 0) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 12:
				__ASSERT((1U << 4) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 13:
				__ASSERT((1U << 15) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 14:
				__ASSERT((1U << 1) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 15:
				__ASSERT((1U << 2) == bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
		#endif
		}
	}
	printf("\nSuccess testing bl_led_get_gpio\n");

	printf("\nTesting bl_led_status_set(), please check for a red led blinking\n");
	 for (int i = 0 ; i < LOOPS ; i++){
		bl_led_status_set(true);
		k_msleep(SLEEP_TIME_MS);
		bl_led_status_set(false);
		k_msleep(SLEEP_TIME_MS);
	}

	bl_led_init();

	printf("\nTesting bl_led_set(), please check for all leds blinking\n");
	 for (int i = 0 ; i < LOOPS ; i++){
		bl_led_set(0xffff);
		k_msleep(SLEEP_TIME_MS);
		bl_led_set(0);
		k_msleep(SLEEP_TIME_MS);
	}
};