idf_component_register(SRCS "main.cpp" 
                    INCLUDE_DIRS "."
                    REQUIRES arduino
                    REQUIRES Sensors
                    REQUIRES IridiumSBD
                    REQUIRES DataLogger
                    REQUIRES GPS
                    REQUIRES LoRa
                    REQUIRES Stats
                    REQUIRES ComBus
                    REQUIRES CommandCenter
                    )


