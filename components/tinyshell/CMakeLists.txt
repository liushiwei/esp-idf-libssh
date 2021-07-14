idf_build_get_property(target IDF_TARGET)

#USB Host is currently only supported on ESP32-S2
if(NOT "${target}" STREQUAL "esp32s2")
    return()
endif()


if(CONFIG_CMD_ECHO)
    list(APPEND srcs "echo.c")
endif()


idf_component_register(SRCS  "wifi.c" "sshd.c" "tinyshell.c" "sshd_task.c" "command.c"
                    REQUIRES libssh
                    INCLUDE_DIRS "." )

