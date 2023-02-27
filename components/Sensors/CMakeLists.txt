idf_component_register(SRCS "Sensors.cpp" "MS5611.cpp" "Thermistor.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES    arduino
                                adafruit
                                ComBus
                    PRIV_REQUIRES uMsg
                    )
