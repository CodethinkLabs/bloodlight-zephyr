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
#include <ztest.h>

#define SLEEP_TIME_MS   1000
#define LOOPS 10

static void test_get_port(void) {

	printf("\nTesting bl_led_get_port\n");
	for (int led = 0; led < BL_LED_COUNT; led++){
		switch (led)
		{
		#if (BL_REVISION == 1)
			case 4:
			case 5:
			case 6:
			case 7:
				zassert_equal(((GPIO_TypeDef *) GPIOA_BASE), bl_led_get_port(led), "Invalid port value in led %i\n", led);
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
				zassert_equal(((GPIO_TypeDef *) GPIOB_BASE), bl_led_get_port(led), "Invalid port value in led %i\n", led);
				break;
			case 8:
			case 9:
			case 10:
				zassert_equal(((GPIO_TypeDef *) GPIOC_BASE), bl_led_get_port(led), "Invalid port value in led %i\n", led);
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
				zassert_equal(((GPIO_TypeDef *) GPIOA_BASE), bl_led_get_port(led), "Invalid port value in led %i\n", led);
					break;
			case 2:
			case 9:
			case 10:
			case 12:
				zassert_equal(((GPIO_TypeDef *) GPIOB_BASE), bl_led_get_port(led), "Invalid port value in led %i\n", led);
				break;
			case 0:
			case 3:
			case 8:
				zassert_equal(((GPIO_TypeDef *) GPIOC_BASE), bl_led_get_port(led), "Invalid port value in led %i\n", led);
				break;
		#endif
		}
	}
		printf("\nSuccess testing bl_led_get_port\n");
}

static void test_get_gpio(void) {
	printf("\nTesting bl_led_get_gpio\n");
	for (int led = 0; led < BL_LED_COUNT; led++){
		switch (led)
		{
		#if (BL_REVISION == 1)
			case 0:
				zassert_equal((1U << 1), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 1:
				zassert_equal((1U << 2), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 2:
				zassert_equal((1U << 10), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 3:
				zassert_equal((1U << 11), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 4:
				zassert_equal((1U << 8), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 5:
				zassert_equal((1U << 9), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 6:
				zassert_equal((1U << 10), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 7:
				zassert_equal((1U << 15), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 8:
				zassert_equal((1U << 13), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 9:
				zassert_equal((1U << 14), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 10:
				zassert_equal((1U << 15), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 11:
				zassert_equal((1U << 9), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 12:
				zassert_equal((1U << 8), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 13:
				zassert_equal((1U << 7), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 14:
				zassert_equal((1U << 6), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 15:
				zassert_equal((1U << 5), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
		#else
			case 0:
				zassert_equal((1U << 14), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 1:
				zassert_equal((1U << 10), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 2:
				zassert_equal((1U << 11), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 3:
				zassert_equal((1U << 13), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 4:
				zassert_equal((1U << 6), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 5:
				zassert_equal((1U << 8), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 6:
				zassert_equal((1U << 9), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 7:
				zassert_equal((1U << 5), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 8:
				zassert_equal((1U << 15), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 9:
				zassert_equal((1U << 6), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 10:
				zassert_equal((1U << 5), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 11:
				zassert_equal((1U << 0), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 12:
				zassert_equal((1U << 4), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 13:
				zassert_equal((1U << 15), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 14:
				zassert_equal((1U << 1), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
			case 15:
				zassert_equal((1U << 2), bl_led_get_gpio(led), "Invalid pin value in led %i\n", led);
				break;
		#endif
		}
	}
	printf("\nSuccess testing bl_led_get_gpio\n");
}

static void test_status(void) {
	printf("\nTesting bl_led_status_set(), please check for a red led blinking\n");
	 for (int i = 0 ; i < LOOPS ; i++){
		bl_led_status_set(true);
		k_msleep(SLEEP_TIME_MS);
		bl_led_status_set(false);
		k_msleep(SLEEP_TIME_MS);
	}
}

static void test_set(void) {
	bl_led_init();

	printf("\nTesting bl_led_set(), please check for all leds blinking\n");
	 for (int i = 0 ; i < LOOPS ; i++){
		bl_led_set(0xffff);
		k_msleep(SLEEP_TIME_MS);
		bl_led_set(0);
		k_msleep(SLEEP_TIME_MS);
	}
}

void test_main (void) {
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

	ztest_test_suite(framework_tests,
		ztest_unit_test(test_get_port),
		ztest_unit_test(test_get_gpio),
		ztest_unit_test(test_set)
	);

	ztest_run_test_suite(framework_tests);

};