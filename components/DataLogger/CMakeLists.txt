idf_component_register(SRCS "datalogger.cpp" "dataline.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES arduino
                    REQUIRES fatfs
                    REQUIRES ComBus
                    REQUIRES uMsg
                    )
