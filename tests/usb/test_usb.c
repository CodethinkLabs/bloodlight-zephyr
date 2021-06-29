#include <stdint.h>
#include <stdio.h>
#include <zephyr.h>
#include <string.h>

#include <logging/log.h>
#include <drivers/uart.h>
#include <usb/usb_device.h>
#include <sys/ring_buffer.h>


#define SLEEP_TIME_MS   1000
#define LOOPS 10
uint8_t ring_buffer[1024];

void main (void) {
	uint32_t dtr = 0;
	const struct device *dev;
	struct ring_buf ringbuf;

	dev = device_get_binding("CDC_ACM_0");
	if (!dev) {
		//LOG_ERR("CDC ACM device not found");
		return;
	}
	
	if (usb_enable(NULL)) {
		return;
	}

	while (!dtr) {
		uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
	}
	printf("pruebaaaaaaaaaaaaaaaaaaaaaaaa\n");
	ring_buf_init(&ringbuf, sizeof(ring_buffer), ring_buffer);

	uart_irq_tx_enable(dev);
	if (uart_irq_tx_ready(dev)) {
		uint8_t buffer[64]= {0,1,2,3,4,5,6,7};;
		int send_len;

		//buffer = {0,1,2,3,4,5,6,7};

		send_len = uart_fifo_fill(dev, buffer, sizeof(buffer));
		if (send_len < sizeof(buffer)) {
			//LOG_ERR("Drop %d bytes", rb_len - send_len);
		}

		//LOG_DBG("ringbuf -> tty fifo %d bytes", send_len);
	}
	
}