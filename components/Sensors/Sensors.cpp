#include "Sensors.h"

void Sensors::vAccelLoop_Task(void *arg)
{
    Sensors sensors = *((Sensors *)arg);

    while (true) // limited / no movement for 5 seconds == kill task
    {
        sensors.log_accel();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

void Sensors::vMainLoop_Task(void *arg)
{
    Sensors sensors = *((Sensors *)arg);
    sensors.setup();

    // Wait for signal to start logging
    while (!sensors.start_data_logging())
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    int count = 0;
    for (;;)
    {
        if (count % 30 == 0)
        {
            count = 0;
            sensors.log_imu_calibration();
        }
        if (sensors.start_accel_logging())
        {
            sensors.create_accel_task();
        }
        sensors.log_data();
        sensors.variable_delay(1000 / sensors.sample_rate_hz);
        count++;
    }
}

bool Sensors::start_accel_logging()
{
    if (umsg_CommandCenter_command_receive(_cmd_sub_handle, &_cmd_data, 1) == pdPASS)
    {
        return _cmd_data.low_alt_data_gather;
    }
    return false;
}

bool Sensors::start_data_logging()
{
    if (umsg_CommandCenter_command_receive(_cmd_sub_handle, &_cmd_data, 1) == pdPASS)
    {
        return _cmd_data.start_system;
    }
    return false;
}

void Sensors::variable_delay(int delay_ms)
{
    while (delay_ms > 0)
    {

        vTaskDelay(1 / portTICK_PERIOD_MS);
        delay_ms -= 1;
    }
}

void Sensors::set_sample_rate_hz(int hz)
{
    sample_rate_hz = hz;
}

void Sensors::log_imu_calibration()
{
    umsg_Sensors_imu_state_t imu_state;
    imu_state.state = SENSOR_OK;
    imu_state.running = 1;
    imu_state.measure_tick = xTaskGetTickCount();

    printf("IMU Calibration:\n");
    uint8_t sys_cal, gyro_cal, accel_cal, mag_cal;
    _imu.getCalibration(&sys_cal, &gyro_cal, &accel_cal, &mag_cal);
    printf("sys: %d, gyro: %d, accel: %d, mag: %d\n", sys_cal, gyro_cal, accel_cal, mag_cal);

    imu_state.calibration_state[0] = gyro_cal;
    imu_state.calibration_state[1] = accel_cal;
    imu_state.calibration_state[2] = mag_cal;

    umsg_Sensors_imu_state_publish(&imu_state);

    uint8_t sys_status,
        self_test_result, system_error;
    _imu.getSystemStatus(&sys_status, &self_test_result, &system_error);
    printf("System Status:\n");
    printf("sys: %d, self_test %d, sys_error: %d\n", sys_status, self_test_result, system_error);
}

void Sensors::create_accel_task()
{
    xTaskCreate(
        Sensors::vAccelLoop_Task,
        "ACCEL_TASK",
        1024 * 2,
        (void *)(this),
        10,
        NULL);
}

/**
 *  SETUP FUNCTIONS
 **/
void Sensors::setup()
{
    sample_rate_hz = 10;

    _gps_sub_handle = umsg_GPS_data_subscribe(1, 1);
    _cmd_sub_handle = umsg_CommandCenter_command_subscribe(1, 1);

    ESP_ERROR_CHECK(setup_imu());
    ESP_ERROR_CHECK(setup_baro());
    ESP_ERROR_CHECK(setup_therm());
    ESP_ERROR_CHECK(setup_accel());
}

esp_err_t Sensors::setup_imu()
{

    umsg_Sensors_imu_state_t state_data;
    state_data.initializing = 1;
    state_data.state = SENSOR_DISCONNECTED;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_imu_state_publish(&state_data);

    position[0] = 0;
    position[1] = 0;
    position[2] = 0;

    velocity[0] = 0;
    velocity[1] = 0;
    velocity[2] = 0;

    imu_last_read = -1; // Use millis because Adafruit libraries use millis()

    _cmd_sub_handle = umsg_CommandCenter_command_subscribe(1, 2);
    _imu = Adafruit_BNO055(55, 0x28, &Wire);
    while (!_imu.begin())
    {
        printf("Could not find a valid MPU6050 Sensor, check wiring!\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    state_data.state = SENSOR_OK;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_imu_state_publish(&state_data);

    // IMU range and scale auto controlled in fusion mode
    // IMU needs to be calibrated first, then record calibration data, and calibrate on boot.

    umsg_Sensors_imu_configuration_t conf_data;
    conf_data.sample_rate_hz = sample_rate_hz;
    conf_data.accelerometer_range = G_4;
    conf_data.gyroscope_scale = DPS_2000;
    conf_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_imu_configuration_publish(&conf_data);

    state_data.initializing = 0;
    state_data.initialized = 1;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_imu_state_publish(&state_data);

    return ESP_OK;
}

esp_err_t Sensors::setup_accel()
{
    umsg_Sensors_accel_state_t state_data;
    state_data.initializing = 1;
    state_data.state = SENSOR_DISCONNECTED;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_accel_state_publish(&state_data);

    _impact_accel = Adafruit_ADXL375(375, &Wire);

    while (!_impact_accel.begin())
    {
        printf("ADXL375 Accelerometer could not be connected\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    state_data.state = SENSOR_OK;
    state_data.initializing = 0;
    state_data.initialized = 1;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_accel_state_publish(&state_data);

    return ESP_OK;
}

esp_err_t Sensors::setup_baro()
{
    umsg_Sensors_baro_state_t state_data;
    state_data.initializing = 1;
    state_data.state = SENSOR_DISCONNECTED;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_baro_state_publish(&state_data);

    _baro = MS5611();

    while (_baro.begin(&Wire) == false)
    {
        printf("MS5611 Barometer could not be connected\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    state_data.state = SENSOR_OK;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_baro_state_publish(&state_data);

    // Allow Barometer to turn on
    vTaskDelay(30 / portTICK_PERIOD_MS);
    _baro.setCompensation(true); // Temperature compensation in pressure measurements.
    if (_baro.read() == MS5611_READ_OK)
    {
        _start_press_hpa = _baro.getPressure();
    }
    else
    {
        printf("Error reading from barometer.\n");
    }

#ifdef ACCURATE_SEA_LEVEL_PRESSURE
    // Wait for an accurate GPS signal.
    uint16_t pdop = -1;
    while (pdop > 700)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
        // Delay so watchdog doesn't get mad
        if (umsg_GPS_data_peek(&_gps_data))
        {
            pdop = _gps_data.p_dilution_precision;
        }
    }
    // Get an altitude reading from GPS
    umsg_GPS_data_receive(_gps_sub_handle, &_gps_data, 1);
    _sea_level_press = ALT_SEA_LEVEL_TRANSITION(current_altitude, _start_press_hpa);

#else
    _sea_level_press = 1013.25; // Value in hpa
#endif

    _start_alt_m = PRESS_ALT_TRANSITION(_start_press_hpa, _sea_level_press);

    umsg_Sensors_baro_configuration_t conf_data;
    conf_data.start_pressure_hpa = _start_press_hpa;
    conf_data.start_alt_m = _start_alt_m;
    conf_data.sea_level_pressure_hpa = _sea_level_press;
    conf_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_baro_configuration_publish(&conf_data);

    state_data.initializing = 0;
    state_data.initialized = 1;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_baro_state_publish(&state_data);

    return ESP_OK;
}

esp_err_t Sensors::setup_therm()
{
    umsg_Sensors_thermistor_state_t therm_state;
    therm_state.initialized = 1;
    therm_state.state = SENSOR_OK;
    therm_state.measure_tick = xTaskGetTickCount();

    umsg_Sensors_thermistor_state_publish_ch(&therm_state, 0);
    umsg_Sensors_thermistor_state_publish_ch(&therm_state, 1);
    return ESP_OK;
}

/**
 *  SENSOR LOGGING
 **/
void Sensors::log_data()
{

    log_imu();
    log_baro();
    log_therm();
}

void Sensors::log_imu()
{

    sensors_event_t accel, mag, gyro, euler_orientation, lin_accel, gravity;

    // Get all events from the imu
    _imu.getEvent(&accel, Adafruit_BNO055::adafruit_vector_type_t::VECTOR_ACCELEROMETER);
    _imu.getEvent(&mag, Adafruit_BNO055::adafruit_vector_type_t::VECTOR_MAGNETOMETER);
    _imu.getEvent(&gyro, Adafruit_BNO055::adafruit_vector_type_t::VECTOR_GYROSCOPE);
    _imu.getEvent(&euler_orientation, Adafruit_BNO055::adafruit_vector_type_t::VECTOR_EULER);
    _imu.getEvent(&lin_accel, Adafruit_BNO055::adafruit_vector_type_t::VECTOR_LINEARACCEL);
    _imu.getEvent(&gravity, Adafruit_BNO055::adafruit_vector_type_t::VECTOR_GRAVITY);

    uint32_t measure_tick = xTaskGetTickCount();

    // Update velocity vector if we have some history
    double speed = 0.0;
    double angle_of_attack = 0.0;
    double vertical_speed = 0.0;
    double horizontal_speed = 0.0;
    if (imu_last_read > 0)
    {

        double ACCEL_VEL_TRANSITION = ACCEL_VEL_TRANSITION_CALC(imu_last_read, lin_accel.timestamp);
        double ACCEL_POS_TRANSITION = ACCEL_POS_TRANSITION_CALC(ACCEL_VEL_TRANSITION);

        // Convert milliseconds to seconds
        float deltaTime = (lin_accel.timestamp - imu_last_read) / 1000;
        float cutoffFrequency = 10;
        float cutoff = 0.2;
        lin_accel.acceleration.x = abs(lin_accel.acceleration.x) > cutoff ? lin_accel.acceleration.x : 0;
        lin_accel.acceleration.y = abs(lin_accel.acceleration.y) > cutoff ? lin_accel.acceleration.y : 0;
        lin_accel.acceleration.z = abs(lin_accel.acceleration.z) > cutoff ? lin_accel.acceleration.z : 0;
        // lin_accel.acceleration.x = _low_pass_filters[0].update(lin_accel.acceleration.x, deltaTime, cutoffFrequency);
        // lin_accel.acceleration.y = _low_pass_filters[1].update(lin_accel.acceleration.x, deltaTime, cutoffFrequency);
        // lin_accel.acceleration.z = _low_pass_filters[2].update(lin_accel.acceleration.x, deltaTime, cutoffFrequency);

        velocity[0] = velocity[0] + (lin_accel.acceleration.x * ACCEL_VEL_TRANSITION);
        velocity[1] = velocity[1] + (lin_accel.acceleration.y * ACCEL_VEL_TRANSITION);
        velocity[2] = velocity[2] + (lin_accel.acceleration.z * ACCEL_VEL_TRANSITION);

        speed = velocity.magnitude();

        // Update Postion vector
        position[0] = position[0] + (lin_accel.acceleration.x * ACCEL_POS_TRANSITION);
        position[1] = position[1] + (lin_accel.acceleration.y * ACCEL_POS_TRANSITION);
        position[2] = position[2] + (lin_accel.acceleration.z * ACCEL_POS_TRANSITION);

        /*Calculate Angle of Attack*/
        // Calculate normal between gravity and z axis. Can be done using cross product
        imu::Vector<3> gravity_vector = imu::Vector<3>(gravity.acceleration.x, gravity.acceleration.y, gravity.acceleration.z);
        gravity_vector.normalize();

        imu::Vector<3> z_vector = imu::Vector<3>(0, 0, 1);

        // We want a unit normal vector
        imu::Vector<3> gz_norm = gravity_vector.cross(z_vector);
        gz_norm.normalize();

        // Project veclocity onto gz_plane. We only care about direction, not magnitude. Normalized vector makes AoA computation
        // More simple.
        imu::Vector<3> proj_velocity_vector = gz_norm.cross(velocity.cross(gz_norm));
        proj_velocity_vector.normalize();

        // Calculate angle between vectors. Magnitude of all vectors is 1
        angle_of_attack = acos(z_vector.dot(proj_velocity_vector));

        // Calculate vertical speed by projecting velocity onto gravity vector. Do not need to divide by
        // magnitude of gravity vector because it is a unit vector
        vertical_speed = velocity.dot(gravity_vector);

        // Calculate horizontal speed by projecting velocity onto horizontal plane formed at (0,0,0) and gravity vector and finding the magnitude
        horizontal_speed = gravity_vector.cross(velocity.cross(gravity_vector)).magnitude();
    }
    umsg_Sensors_imu_data_t data;

    data.accelerometer[0] = accel.acceleration.x;
    data.accelerometer[1] = accel.acceleration.y;
    data.accelerometer[2] = accel.acceleration.z;

    data.gyroscope[0] = gyro.gyro.x;
    data.gyroscope[1] = gyro.gyro.y;
    data.gyroscope[2] = gyro.gyro.z;

    data.magnetometer[0] = mag.magnetic.x;
    data.magnetometer[1] = mag.magnetic.y;
    data.magnetometer[2] = mag.magnetic.z;

    data.attitude[0] = euler_orientation.orientation.x;
    data.attitude[1] = euler_orientation.orientation.y;
    data.attitude[2] = euler_orientation.orientation.z;

    data.linear_accel[0] = lin_accel.acceleration.x;
    data.linear_accel[1] = lin_accel.acceleration.y;
    data.linear_accel[2] = lin_accel.acceleration.z;

    data.gravity[0] = gravity.acceleration.x;
    data.gravity[1] = gravity.acceleration.y;
    data.gravity[2] = gravity.acceleration.z;

    data.velocity[0] = velocity[0];
    data.velocity[1] = velocity[1];
    data.velocity[2] = velocity[2];

    data.position[0] = position[0];
    data.position[1] = position[1];
    data.position[2] = position[2];

    data.angle_of_attack = angle_of_attack;
    data.vertical_speed = vertical_speed;
    data.horizontal_speed = horizontal_speed;
    data.speed = speed;

    data.temperature_c = _imu.getTemp();
    data.measure_tick = measure_tick;
    imu_last_read = lin_accel.timestamp;

    umsg_Sensors_imu_data_publish(&data);
}

void Sensors::log_baro()
{
    umsg_Sensors_baro_data_t data;
    if (_baro.read() != MS5611_READ_OK)
    {
        printf("Problem reading from barometer!\n");
        return;
    }

    float temp = _baro.getTemperature();
    float pressure = _baro.getPressure(); // Pressure in millibar / hPa
    data.measure_tick = _baro.lastRead();

    // // Standard pressure of sea level: 1013.25
    // // Could be pressure at launch site to estimate hight above ground

    float gained_alt_m = PRESS_ALT_TRANSITION(pressure, _start_press_hpa);
    float alt_above_sea_m = PRESS_ALT_TRANSITION(pressure, _sea_level_press);

    data.pressure_pa = pressure;
    data.temperature_c = temp;
    data.gained_alt_m = gained_alt_m;
    data.alt_above_sea_m = alt_above_sea_m;

    umsg_Sensors_baro_data_publish(&data);
}

// #define therm_attached

void Sensors::log_therm()
{
    Thermistors::readThermistors(_internal_thermistor_c, _external_thermistor_c);
    uint32_t measure_tick = xTaskGetTickCount();
    umsg_Sensors_thermistor_data_t internal_data;
    umsg_Sensors_thermistor_data_t external_data;

#ifdef therm_attached
    internal_data.temperature_c = _internal_thermistor_c;
#else
    internal_data.temperature_c = 0;
#endif
    internal_data.measure_tick = measure_tick;
    umsg_Sensors_thermistor_data_publish_ch(&internal_data, 0);

#ifdef therm_attached
    external_data.temperature_c = _external_thermistor_c;
#else
    external_data.temperature_c = 0;
#endif
    external_data.measure_tick = measure_tick;
    umsg_Sensors_thermistor_data_publish_ch(&external_data, 1);
}

void Sensors::log_accel()
{
    sensors_event_t event;
    _impact_accel.getEvent(&event);

    umsg_Sensors_accel_data_t accel_data;
    accel_data.acceleration[0] = event.acceleration.x;
    accel_data.acceleration[1] = event.acceleration.y;
    accel_data.acceleration[2] = event.acceleration.z;
    accel_data.measure_tick = xTaskGetTickCount();
}
