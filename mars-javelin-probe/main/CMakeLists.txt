idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES IridiumSBD
                    REQUIRES BME280
                    REQUIRES BMI085
                    REQUIRES DataLogger
                    REQUIRES GPS
                    REQUIRES LoRa
                    )


