idf_component_register(SRCS "ComBus.cpp" "SC16IS752.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES LoRa
                    REQUIRES fatfs
                    REQUIRES arduino)
