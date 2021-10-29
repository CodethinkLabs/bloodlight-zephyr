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

#include <drivers/uart.h>
#include <usb/usb_device.h>
#include <sys/ring_buffer.h>

#include "common/error.h"
#include "common/util.h"

#include "msg.h"
#include "mq.h"
#include "usb.h"

struct ring_buf ringbuf;
uint8_t buffer[sizeof(ringbuf)];
union bl_msg_data *msg;

static bool usb_response_used;
static union bl_msg_data usb_response;

/**
 * Send a message to the host.
 *
 * \param[in]  msg  Message to send to host.
 * \return True on success.
 */
static bool bl_usb__send_message(union bl_msg_data *msg, const struct device *dev)
{
	uint16_t send_len = 0;
	uint16_t len = bl_msg_len(msg);

	send_len = uart_fifo_fill(dev, (uint8_t *) msg, len);
	if (send_len < sizeof(buffer)) {
		return false;
	}

	return true;
}

static bool bl_usb__receive_message(union bl_msg_data *msg, const struct device *dev)
{
	uint16_t recv_len = 0;
	uint16_t len = bl_msg_len(msg);

	
	recv_len = uart_fifo_read(dev, (uint8_t *) msg, len);
	if (recv_len < len)
		return false;

	msg = bl_msg_decode((uint8_t *) msg, len);
	if (msg == NULL) {
		usb_response.response.type        = BL_MSG_RESPONSE;
		usb_response.response.response_to = bl_msg_get_type(msg);
		usb_response.response.error_code  = BL_ERROR_BAD_MESSAGE_LENGTH;
		usb_response_used = true;
		return false;
	}

	usb_response_used = bl_msg_handle(msg, &usb_response);

	return true;
}

static void interrupt_handler(const struct device *dev, void *user_data)
{
	ARG_UNUSED(user_data);
	if (uart_irq_rx_ready(dev))
	{
		bl_usb__receive_message(msg,dev);
	}
}

bool bl_usb_init(void)
{
	const struct device *dev;

	dev = device_get_binding("CDC_ACM_0");
	if (!dev) {
		return false;
	}

	if (usb_enable(NULL)) {
		return false;
	}

	uart_irq_callback_set(dev, interrupt_handler);
	uart_irq_rx_enable(dev);
	return true;
}

/* Exported function, documented in usb.h */
void bl_usb_poll(void)
{
	const struct device *dev;
	dev = device_get_binding("CDC_ACM_0");

	if (mq_pending != 0x00) {
		unsigned channel = bl_mq_pending_channel();

		union bl_msg_data *msg = bl_mq_peek(channel);
		if (msg && bl_usb__send_message(msg, dev)) {
			bl_mq_release(channel);
		}
	} else if (usb_response_used) {
		if (bl_usb__send_message((union bl_msg_data *)&usb_response, dev)) {
			usb_response_used = false;
		}
	}
}
