#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "command.h"
#include "tinyshell.h"
#include "string.h"


#define CMD_UART_TXD 42
#define CMD_UART_RXD 41
#define CMD_UART_RTS (UART_PIN_NO_CHANGE)
#define CMD_UART_CTS (UART_PIN_NO_CHANGE)

#define UART_PORT_NUM      1
#define UART_BAUD_RATE     115200
#define TASK_STACK_SIZE    3072

#define BUF_SIZE (256)
extern xQueueHandle sshd_rx_queue;  //需要显示给sshd客户端的内容
extern xQueueHandle sshd_tx_queue;  //sshd客户端的内容
extern struct interactive_session * sshd_session;
static void uart_handle_output(struct interactive_session *is);
int cancel_uart=0;
int is_uart_start=0;
/**
 * @brief george 输入解析字符
 */
static void
uart_handle_input(struct interactive_session *is, char c)
{
    if (c == 24) {
		// ^X
		minicli_printf(is, "^C\n");
		cancel_uart =1;
	}
    char tmp[2]={0,0};
    tmp[0] = c;
    ESP_LOGI("CMD_UART", "uart_handle_input to uart %s",(char *)&c);
	uart_write_bytes(UART_PORT_NUM, (const char *) &c, 1);
}
void cmd_uart_task(void *arg){
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    ESP_LOGI("CMD_UART", "uart_init start");
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
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

    ESP_ERROR_CHECK(uart_driver_install(UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT_NUM, CMD_UART_TXD, CMD_UART_RXD, CMD_UART_RTS, CMD_UART_CTS));


      uint8_t *data = (uint8_t *) malloc(BUF_SIZE);
    // ESP_LOGI("CMD_UART", "uart init end");
    while (!cancel_uart) {
        memset(data,0,BUF_SIZE);
        // ESP_LOGI("CMD_UART", "uart read");
        // Read data from the UART
        int len = uart_read_bytes(UART_PORT_NUM, data, BUF_SIZE, 20 / portTICK_RATE_MS);
        // Write data back to the UART
        // ESP_LOGI("CMD_UART", "uart get data %d",len);
        if(len>0&&is_uart_start){
        ESP_LOGI("CMD_UART", "uart write to ssh %s",(char *)data);
        // minicli_printf((struct interactive_session *)is,(char *)data);
        for(int i=0;i<len;i++){
            if (data[i] == '\r') {
                data[i] = '\n';
            }
            xQueueSend(sshd_rx_queue,&data[i],NULL);
        }
        }
        // ESP_LOGI("CMD_UART", "uart write to ssh end");
    }
}

int do_uart(struct interactive_session * ss,cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    ESP_LOGI("CMD_UART", "do_uart start");
    ss->is_handle_char_from_remote = uart_handle_input;
    is_uart_start = 1;
    ESP_LOGI("CMD_UART", "do_uart end");
    return 0;
}

