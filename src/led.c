/*
 * Copyright 2020 Codethink Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stddef.h>
#include <stdint.h>

#include <drivers/clock_control.h>
#include <drivers/gpio.h>

#include "common/error.h"
#include "common/util.h"
#include "common/msg.h"

#include "led.h"
#include "acq/channel.h"
//Commentend to avoid depending on spi, to be uncommented
//#include "spi.h"
#include "acq.h"

#if (BL_REVISION == 1)
#define NON_LED_MASKS (1 << BL_ACQ_3V3 | \
                       1 << BL_ACQ_5V0 | \
                       1 << BL_ACQ_TMP)
#else
#define NON_LED_MASKS (1 << BL_ACQ_3V3 | \
                       1 << BL_ACQ_5V0 | \
                       1 << BL_ACQ_TMP | \
                       1 << BL_ACQ_EXT)
#endif

#define LED_SRC(pd)    (1U << (pd) | NON_LED_MASKS)
#define LED_BS(pin)    (1U << (pin))
#define LED_BR(pin)    (LED_BS(pin) << 16)
#define LED_BSRR(port) ((port) + 0x18)

/* The devicetree node identifier for the "led0" alias. */
#define STATUS_NODE DT_ALIAS(statusled)

#if DT_NODE_HAS_STATUS(STATUS_NODE, okay)
#define STATUS_LED    DT_GPIO_LABEL(STATUS_NODE, gpios)
#define STATUS_PIN     DT_GPIO_PIN(STATUS_NODE, gpios)
#define STATUS_FLAGS   DT_GPIO_FLAGS(STATUS_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: statusled devicetree alias is not defined"
#define STATUS_LED    ""
#define STATUS_PIN     0
#define STATUS_FLAGS   0
#endif

unsigned bl_led_count;
volatile unsigned bl_led_active;
bl_led_channel_t bl_led_channel[BL_LED_COUNT];

/** GPIO ports used for LEDs */
enum led_port {
	LED_PORT_A,
	LED_PORT_B,
	LED_PORT_C,
};

/** GPIO port addresses */
static const GPIO_TypeDef * led_port[] = {
	[LED_PORT_A] = GPIOA,
	[LED_PORT_B] = GPIOB,
	[LED_PORT_C] = GPIOC,
};

/** Table of LED ports/pins. */
static const struct led_table {
	uint8_t port_idx;
	uint8_t pin;
} led_table[BL_LED_COUNT] = {
#if (BL_REVISION == 1)
	[ 0] = { .port_idx = LED_PORT_B, .pin =  1 },
	[ 1] = { .port_idx = LED_PORT_B, .pin =  2 },
	[ 2] = { .port_idx = LED_PORT_B, .pin = 10 },
	[ 3] = { .port_idx = LED_PORT_B, .pin = 11 },
	[ 4] = { .port_idx = LED_PORT_A, .pin =  8 },
	[ 5] = { .port_idx = LED_PORT_A, .pin =  9 },
	[ 6] = { .port_idx = LED_PORT_A, .pin = 10 },
	[ 7] = { .port_idx = LED_PORT_A, .pin = 15 },
	[ 8] = { .port_idx = LED_PORT_C, .pin = 13 },
	[ 9] = { .port_idx = LED_PORT_C, .pin = 14 },
	[10] = { .port_idx = LED_PORT_C, .pin = 15 },
	[11] = { .port_idx = LED_PORT_B, .pin =  9 },
	[12] = { .port_idx = LED_PORT_B, .pin =  8 },
	[13] = { .port_idx = LED_PORT_B, .pin =  7 },
	[14] = { .port_idx = LED_PORT_B, .pin =  6 },
	[15] = { .port_idx = LED_PORT_B, .pin =  5 },
#else
	[ 0] = { .port_idx = LED_PORT_C, .pin = 14 },
	[ 1] = { .port_idx = LED_PORT_A, .pin = 10 },
	[ 2] = { .port_idx = LED_PORT_B, .pin = 11 },
	[ 3] = { .port_idx = LED_PORT_C, .pin = 13 },
	[ 4] = { .port_idx = LED_PORT_A, .pin =  6 },
	[ 5] = { .port_idx = LED_PORT_A, .pin =  8 },
	[ 6] = { .port_idx = LED_PORT_A, .pin =  9 },
	[ 7] = { .port_idx = LED_PORT_A, .pin =  5 },
	[ 8] = { .port_idx = LED_PORT_C, .pin = 15 },
	[ 9] = { .port_idx = LED_PORT_B, .pin =  6 },
	[10] = { .port_idx = LED_PORT_B, .pin =  5 },
	[11] = { .port_idx = LED_PORT_A, .pin =  0 },
	[12] = { .port_idx = LED_PORT_B, .pin =  4 },
	[13] = { .port_idx = LED_PORT_A, .pin = 15 },
	[14] = { .port_idx = LED_PORT_A, .pin =  1 },
	[15] = { .port_idx = LED_PORT_A, .pin =  2 },
#endif
};

static inline uint16_t bl_led__get_pin_mask(
		enum led_port port,
		uint16_t led_mask)
{
	uint16_t pin_mask = 0;

	for (unsigned i = 0; i < BL_LED_COUNT; i++) {
		if ((1U << i) & led_mask) {
			if (led_table[i].port_idx == port) {
				pin_mask |= (1 << led_table[i].pin);
			}
		}
	}

	return pin_mask;
}

/** GPIO binding function, needed to avoid variables holding desired node value*/
static inline const struct device * gpio_binding (enum led_port port) {
	const struct device * gpio;
	switch (port) {
		case LED_PORT_A:
			gpio = device_get_binding(DT_LABEL(DT_NODELABEL(gpioa)));
			if (gpio == NULL) {
				printk("GPIOA binding error\n");
			}
			break;
		case LED_PORT_B:
			gpio = device_get_binding(DT_LABEL(DT_NODELABEL(gpiob)));
			if (gpio == NULL) {
				printk("GPIOB binding error\n");
			}
			break;
		case LED_PORT_C:
			gpio = device_get_binding(DT_LABEL(DT_NODELABEL(gpioc)));
			if (gpio == NULL) {
				printk("GPIOC binding error\n");
			}
			break;
		default:
			gpio = NULL; //check NULL
			break;
	}
	return gpio;
}



static inline void bl_led__gpio_mode_setup()
{
	//Get the gpio bindings
	const struct device * gpioa = gpio_binding(LED_PORT_A);
	const struct device * gpiob = gpio_binding(LED_PORT_B);
	const struct device * gpioc = gpio_binding(LED_PORT_C);

	//Configure the pins connected to leds as output.
	for (int led = 0; led < BL_LED_COUNT ; led++) {
		switch (led_table[led].port_idx){
		case LED_PORT_A:
				if (gpio_pin_configure(gpioa, led_table[led].pin, GPIO_OUTPUT) !=0){
					printk("Error configuring port\n");
				}
				break;
		case LED_PORT_B:
				if (gpio_pin_configure(gpiob, led_table[led].pin, GPIO_OUTPUT) !=0){
					printk("Error configuring port\n");
				}
				break;
		case LED_PORT_C:
				if (gpio_pin_configure(gpioc, led_table[led].pin, GPIO_OUTPUT) !=0){
					printk("Error configuring port\n");
				}
				break;
		}

	}
}

/* Exported function, documented in led.h */
void bl_led_init(void)
{
	for (uint8_t port = 0 ; port < sizeof(led_port)/sizeof(led_port[0]); port++) {
		const struct device * gpio = gpio_binding(port);

		const clock_control_subsys_t *subsys = gpio -> config;
		if (clock_control_on(gpio, *subsys) < 0) {
			printk("Error: Can't turn clock on\n");
		}
	};

	bl_led__gpio_mode_setup();

	bl_led_set(0x0000);
}

static inline void bl_led__set(
		enum led_port port,
		uint16_t led_mask)
{
	/* We can trivially optimise this, if needed, by:
	 *
	 * 1. Making bl_led__get_pin_mask() build all three port masks at once.
	 * 2. Making bl_led_init() cache the clear masks so they can be reused
	 *    here.
	 */
	typedef uint32_t gpio_port_pins_t;
	const struct device * gpio;

	gpio_port_pins_t pinmask = bl_led__get_pin_mask(port, 0xffff);
	gpio = gpio_binding(port);
	gpio_port_set_masked(gpio, pinmask, 0);
	pinmask = bl_led__get_pin_mask(port, led_mask);
	gpio_port_set_masked(gpio, pinmask, 0xffff);
}

/* Exported function, documented in led.h */
enum bl_error bl_led_set(uint16_t led_mask)
{
	bl_led__set(LED_PORT_A, led_mask);
	bl_led__set(LED_PORT_B, led_mask);
	bl_led__set(LED_PORT_C, led_mask);

	return BL_ERROR_NONE;
}

void bl_led_status_set(bool enable)
{
#if (BL_REVISION >= 2)
	const struct device * gpio;

	gpio=device_get_binding(STATUS_LED);
	if (gpio == NULL) {
		return;
	}

	if (enable) {
		if (gpio_pin_configure(gpio, STATUS_PIN, GPIO_OUTPUT_LOW | STATUS_FLAGS) < 0) {
			printk("Error: failed to enable status pin\n");
			return;
		}
	} else {
		if (gpio_pin_configure(gpio, STATUS_PIN, GPIO_INPUT | STATUS_FLAGS) < 0) {
			printk("Error: failed to disable status pin\n");
			return;
		}
	}
#else
	BL_UNUSED(enable);
#endif
}

/* Exported function, documented in led.h */
enum bl_error bl_led_setup(uint16_t led_mask)
{
	if (!led_mask) {
		return BL_ERROR_BAD_SOURCE_MASK;
	}

	bl_led_active = 0;
	bl_led_count = 0;

	while (led_mask)
	{
		unsigned i = __builtin_ffs(led_mask) - 1;
		bl_led_channel_t *active = &bl_led_channel[bl_led_active];

		active->led = i;
		active->src_mask = LED_SRC(bl_acq_channel_get_source(i));
		active->gpios = LED_BS(led_table[i].pin);
		active->gpior = LED_BR(led_table[i].pin);
		active->gpio_bsrr = LED_BSRR(led_port[led_table[i].port_idx]->BSRR);

		led_mask &= ~(1U << i);

		bl_led_active++;
		bl_led_count++;
	}

	bl_led_active = 0;

	return BL_ERROR_NONE;
}

/* These 2 inline functions replaces the libopencm3 gpio_clear/set
   APIs to avoid function call overhead in IRQ handler */
static inline void bl_led__gpio_clear(unsigned led)
{
	*(volatile uint32_t *)bl_led_channel[led].gpio_bsrr = bl_led_channel[led].gpior;
}

static inline void bl_led__gpio_set(unsigned led)
{
	*(volatile uint32_t *)bl_led_channel[led].gpio_bsrr = bl_led_channel[led].gpios;
}

/* Exported function, documented in led.h */
enum bl_error bl_led_loop(void)
{
	const struct device * gpio;

	gpio = device_get_binding(DT_LABEL(DT_NODELABEL(gpiob)));
	if (gpio_pin_configure(gpio, 12, GPIO_OUTPUT | STATUS_FLAGS) < 0) {
		printk("Error: failed to configure pin 12\n");
		return BL_ERROR_HARDWARE_CONFLICT;
	}
	gpio_port_set_bits(gpio, 12);
	//Commented to avoid depending on spi, to be uncommented
	/*if (bl_spi_mode == BL_ACQ_SPI_NONE) {
		bl_led__gpio_clear(bl_led_active);
	}*/

	bl_led_active++;
	if (bl_led_active >= bl_led_count)
		bl_led_active = 0;

	//Commented to avoid depending on spi, to be uncommented
	/*if (bl_spi_mode == BL_ACQ_SPI_MOTHER) {
		unsigned led_to_send = bl_led_active + 1;
		if (led_to_send >= bl_led_count)
			led_to_send = 0;

		bl_spi_send(bl_led_channel[led_to_send].led);
		gpio_port_clear_bits(gpio, GPIO12);
	} else if (bl_spi_mode == BL_ACQ_SPI_NONE) {
		bl_led__gpio_set(bl_led_active);
	}*/

	return BL_ERROR_NONE;
}

/* Exported function, documented in led.h */
const GPIO_TypeDef * bl_led_get_port(uint8_t led)
{
	return led_port[led_table[led].port_idx];
}

/* Exported function, documented in led.h */
uint16_t bl_led_get_gpio(uint8_t led)
{
	return (1U << led_table[led].pin);
}
