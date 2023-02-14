idf_component_register(SRCS "IMUComponent.cpp" "MPU6050.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES DataLogger
                    REQUIRES ComBus
                    REQUIRES kalman_filter
                    )
