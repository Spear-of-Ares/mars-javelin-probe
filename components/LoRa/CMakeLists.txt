idf_component_register(SRCS "LoRa.cpp" "LoRaComponent.cpp"
                       INCLUDE_DIRS "include"
                       REQUIRES arduino
                       REQUIRES ComBus
                       REQUIRES DataLogger
                       )
