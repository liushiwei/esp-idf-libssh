idf_build_get_property(target IDF_TARGET)

#USB Host is currently only supported on ESP32-S2
if(NOT "${target}" STREQUAL "esp32s2")
    return()
endif()
message(STATUS "tinyshell =========")

set(srcs "wifi.c" "sshd.c" "tinyshell.c" "sshd_task.c" "command.c" )

if(CONFIG_CMD_ECHO)
    list(APPEND srcs "cmd_echo.c")
endif()
if(CONFIG_CMD_UART)
    message(STATUS "tinyshell APPEND cmd_uart.c =========")
    list(APPEND srcs "cmd_uart.c")
endif()


idf_component_register(SRCS  ${srcs} 
                    REQUIRES libssh
                    INCLUDE_DIRS "." )

