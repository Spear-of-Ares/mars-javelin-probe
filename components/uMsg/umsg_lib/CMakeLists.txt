# create library
add_library(uMsgLib STATIC
    core/src/umsg.c
    # generated files
    src/CommandCenter.c
    src/GPS.c
    src/Iridium.c
    src/LoRa.c
    src/Sensors.c
    src/Stats.c
    src/StatusMsgs.c
)
# select port
if(NOT DEFINED UMSG_PORT)
    message(FATAL_ERROR "UMSG_PORT is not defined")
endif()

if("${UMSG_PORT}" STREQUAL "FREERTOS")
    target_sources(uMsgLib PRIVATE
            core/src/port_freertos.c
    )
elseif("${UMSG_PORT}" STREQUAL "POSIX")
        target_sources(uMsgLib PRIVATE
                core/src/port_posix.c
        )
endif()

# add includes
target_include_directories(uMsgLib PUBLIC inc core/inc)
target_link_options(uMsgLib PRIVATE -ffunction-sections -fdata-sections)
# linker options to allow dead code elimination
target_link_options(uMsgLib PRIVATE -Wl,--gc-sections)