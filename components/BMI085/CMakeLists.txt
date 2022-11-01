idf_component_register(SRCS "BMI085.cpp" "IMUComponent.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES DataLogger
                    REQUIRES ComBus
                    )
