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

#include <stdbool.h>

#include "common/error.h"

#include "acq.h"
#include "led.h"
#include "bl.h"
#include "usb.h"
#include "tick.h"
#include "delay.h"
#include "spi.h"

/*Definitions to enable power interface clock*/
#define RCC_APB1ENR1 0x58
#define WRITE_RCC_APB1ENR1(val) ((*(volatile uint32_t *)RCC_APB1ENR1) = (val))
#define READ_RCC_APB1ENR1()     (*(volatile uint32_t *)RCC_APB1ENR1)
#define RCC_PWR 28

/*Definitions to enable flash memory interface clock*/
#define RCC_AHB1ENR 0x48
#define WRITE_RCC_AHB1ENR(val) ((*(volatile uint32_t *)RCC_AHB1ENR) = (val))
#define READ_RCC_AHB1ENR()     (*(volatile uint32_t *)RCC_AHB1ENR)
#define RCC_FLASH 8

/*Definitions to enable power interface clock*/
#define RCC_APB2ENR 0x60
#define WRITE_RCC_APB2ENR(val) ((*(volatile uint32_t *)RCC_APB2ENR) = (val))
#define READ_RCC_APB2ENR()     (*(volatile uint32_t *)RCC_APB2ENR)
#define RCC_SYSCFGEN 0


enum bl_acq_spi_mode bl_spi_mode = BL_ACQ_SPI_INIT;

#if (BL_REVISION == 1)
const struct rcc_clock_scale rcc_hse_16mhz_3v3 = {
	.pllsrc = RCC_CFGR_PLLSRC_HSE_PREDIV,
	.pllmul = RCC_CFGR_PLLMUL_MUL9,
	.plldiv = RCC_CFGR2_PREDIV_DIV2,
	.usbdiv1 = false,
	.flash_waitstates = 2,
	.hpre = RCC_CFGR_HPRE_DIV_NONE,
	.ppre1 = RCC_CFGR_PPRE1_DIV_2,
	.ppre2 = RCC_CFGR_PPRE2_DIV_NONE,
	.ahb_frequency = 72e6,
	.apb1_frequency = 36e6,
	.apb2_frequency = 72e6,
};
#endif

/* Exported function, documented in bl.h */
void bl_init(void)
{
	//const struct rcc_clock_scale *rcc_config;

#if (BL_REVISION == 1)
	//rcc_config = &rcc_hse_16mhz_3v3;
#else
	WRITE_RCC_APB1ENR1(RCC_PWR);
	WRITE_RCC_AHB1ENR(RCC_FLASH);
	WRITE_RCC_APB2ENR(RCC_SYSCFGEN);
#endif

	//bl_tick_init(rcc_config->ahb_frequency);
	bl_usb_init();
	bl_led_init();
	//bl_acq_init(rcc_config->ahb_frequency);
	//bl_spi_init();
}

#ifndef BL_SPI_TEST
int main(void)
{
	bl_init();

	bl_led_status_set(true);

	while (true) {
		if (bl_spi_mode == BL_ACQ_SPI_INIT) {
			bl_spi_daughter_poll();
			bl_usb_poll();
		} else if (bl_spi_mode == BL_ACQ_SPI_DAUGHTER) {
			bl_spi_daughter_poll();
		} else {
			bl_usb_poll();
		}
	}
}
#endif
