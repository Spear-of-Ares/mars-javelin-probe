idf_component_register(SRCS "main.cpp" 
                    INCLUDE_DIRS "."
                    REQUIRES arduino
                    REQUIRES IridiumSBD
                    REQUIRES BME280
                    REQUIRES MPU6050
                    REQUIRES DataLogger
                    REQUIRES GPS
                    REQUIRES LoRa
                    REQUIRES Stats
                    REQUIRES ComBus
                    REQUIRES Thermistor
                    REQUIRES CommandCenter
                    )


