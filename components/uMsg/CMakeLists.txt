# # Add uMSG
idf_component_register(SRCS     "umsg_lib/core/src/umsg.c" 
                                "umsg_lib/core/src/port_freertos.c" 
                                "umsg_lib/src/CommandCenter.c" 
                                "umsg_lib/src/GPS.c" 
                                "umsg_lib/src/Iridium.c" 
                                "umsg_lib/src/LoRa.c" 
                                "umsg_lib/src/Sensors.c" 
                                "umsg_lib/src/Stats.c" 
                                "umsg_lib/src/StatusMsgs.c" 
                                "Sensors_toDataLine.cpp"
                                "LoRa_toDataLine.cpp"
                                "GPS_toDataLine.cpp"
                        INCLUDE_DIRS "umsg_lib/inc" "umsg_lib/core/inc" "include"
                        PRIV_REQUIRES freertos
                        PRIV_REQUIRES DataLogger
        )
target_link_options(${COMPONENT_LIB} PRIVATE -ffunction-sections -fdata-sections)
# linker options to allow dead code elimination
target_link_options(${COMPONENT_LIB} PRIVATE -Wl,--gc-sections)

