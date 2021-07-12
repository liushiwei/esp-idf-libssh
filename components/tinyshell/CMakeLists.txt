idf_build_get_property(target IDF_TARGET)

#USB Host is currently only supported on ESP32-S2
if(NOT "${target}" STREQUAL "esp32s2")
    return()
endif()

idf_component_register(SRCS "wifi.c" "sshd.c" "tinyshell.c" "sshd_task.c"
                    REQUIRES libssh
                    INCLUDE_DIRS "." )

