idf_component_register(SRCS "main.cpp" 
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES   arduino
                                    Sensors
                                    RFD900
                                    DataLogger
                                    GPS
                                    LoRa
                                    Stats
                                    ComBus
                                    CommandCenter 
                                    uMsg
                    )



