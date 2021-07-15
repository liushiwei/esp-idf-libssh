#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "command.h"
#include "tinyshell.h"

#define CONFIG_EXAMPLE_UART_PORT_NUM 1
#define CONFIG_EXAMPLE_UART_BAUD_RATE 115200
#define CONFIG_EXAMPLE_UART_RXD 5
#define CONFIG_EXAMPLE_UART_TXD 4
#define CONFIG_EXAMPLE_TASK_STACK_SIZE 2048

#define ECHO_TEST_TXD (CONFIG_EXAMPLE_UART_TXD)
#define ECHO_TEST_RXD (CONFIG_EXAMPLE_UART_RXD)
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

#define ECHO_UART_PORT_NUM      (CONFIG_EXAMPLE_UART_PORT_NUM)
#define ECHO_UART_BAUD_RATE     (CONFIG_EXAMPLE_UART_BAUD_RATE)
#define ECHO_TASK_STACK_SIZE    (CONFIG_EXAMPLE_TASK_STACK_SIZE)

#define BUF_SIZE (1024)

static void echo_task(void *arg);
int cancel_uart=0;

/**
 * @brief george 输入解析字符
 */
static void
minicli_handle_char(struct interactive_session *is, char c)
{
    if (c == 24) {
		// ^X
		minicli_printf(is, "^C\n");
		cancel_uart =1;
	}
	uart_write_bytes(ECHO_UART_PORT_NUM, (const char *) c, 1);
}

int do_uart(struct interactive_session * ss,cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    ss->is_handle_char_from_remote = minicli_handle_char;
    xTaskCreate(echo_task, "uart_echo_task", ECHO_TASK_STACK_SIZE, ss, 10, NULL);
    return 0;
}


static void echo_task(void *arg)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = ECHO_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));

    // Configure a temporary buffer for the incoming data
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

    while (!cancel_uart) {
        // Read data from the UART
        int len = uart_read_bytes(ECHO_UART_PORT_NUM, data, BUF_SIZE, 20 / portTICK_RATE_MS);
        // Write data back to the UART
        minicli_printf((struct interactive_session *)arg,(char *)data);
    }
}
