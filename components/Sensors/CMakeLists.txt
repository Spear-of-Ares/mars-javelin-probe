idf_component_register(SRCS "Sensors.cpp" "MPU6050.cpp" "Thermistor.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES    arduino
                                adafruit
                                ComBus
                    PRIV_REQUIRES uMsg
                    )
