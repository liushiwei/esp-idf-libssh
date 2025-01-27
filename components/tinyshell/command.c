#include "command.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#ifdef CONFIG_CMD_ECHO
extern int do_echo(struct interactive_session * ss,cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
#ifdef CONFIG_CMD_UART
extern void cmd_uart_task(void *arg);
extern int do_uart(struct interactive_session * ss,cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
#endif
cmd_tbl_t commands[] = {

#ifdef CONFIG_CMD_ECHO
        { "echo",   CONFIG_SYS_MAXARGS,     1,      do_echo,
        "echo args to console",
        "[args..]\n"
        "    - echo args to console; \\c suppresses newline"
        },
#endif
#ifdef CONFIG_CMD_UART
        { "uart",   CONFIG_SYS_MAXARGS,     1,      do_uart,
        "start uart connect",
        "[args..]\n"
        "    - start uart connect"
        },
#endif
    { NULL,   CONFIG_SYS_MAXARGS,     1,      NULL,
        NULL,
        "[args..]\n"
        "    - echo args to console; \\c suppresses newline"
        }

};


_Bool cmd_init(){
    // init_arr(&commands,MAX_CMD_NUM);
    #ifdef CONFIG_CMD_UART
    xTaskCreate(cmd_uart_task, "cmd_uart_task", 2048, NULL, 10, NULL);
    #endif
    return true;
}