#include "Sensors.h"

// void Sensors::vAccelLoop_Task(void *arg)
// {
//     Sensors sensors = *((Sensors *)arg);
//     sensors.setup();

//     while (true) // limited / no movement for 5 seconds == kill task
//     {
//         // sensors.log_accel();
//         vTaskDelay(1 / portTICK_PERIOD_MS);
//     }
// }

void Sensors::vMainLoop_Task(void *arg)
{
    Sensors sensors = *((Sensors *)arg);
    sensors.setup();
    for (;;)
    {
        sensors.log_data();
        sensors.variable_delay(1000 / sensors.sample_rate_hz);
    }
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

/**
 *  SETUP FUNCTIONS
 **/
void Sensors::setup()
{
    sample_rate_hz = 10;

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

    cmd_sub_handle = umsg_CommandCenter_command_subscribe(1, 2);
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
    state_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_imu_state_publish(&state_data);

    return ESP_OK;
}

esp_err_t Sensors::setup_accel()
{
    //_impact_accel = Adafruit_ADXL375(375, &Wire);

    return ESP_OK;
}

esp_err_t Sensors::setup_baro()
{
    // umsg_Sensors_baro_state_t state_data;
    // state_data.initializing = 1;
    // state_data.state = SENSOR_DISCONNECTED;
    // state_data.measure_tick = xTaskGetTickCount();
    // umsg_Sensors_baro_state_publish(&state_data);

    // _bme = Adafruit_BME280();
    // while (_bme.begin((uint8_t)BME280_ADDRESS, &Wire) == false)
    // {
    //     printf("BME280 could not be connected\n");
    // }

    // state_data.state = SENSOR_OK;
    // state_data.measure_tick = xTaskGetTickCount();
    // umsg_Sensors_baro_state_publish(&state_data);

    // float start_press_pa = _bme.readPressure(); // pressue in Pa
    // _start_press_hpa = start_press_pa / 100.0;  // Convert to hPa
    // _start_alt_m = _bme.readAltitude(1013.25);

    // umsg_Sensors_baro_configuration_t conf_data;
    // conf_data.start_pressure_hpa = _start_press_hpa;
    // conf_data.start_alt_m = _start_alt_m;
    // conf_data.measure_tick = xTaskGetTickCount();
    // umsg_Sensors_baro_configuration_publish(&conf_data);

    // state_data.initializing = 0;
    // state_data.logging_data = 1;
    // state_data.measure_tick = xTaskGetTickCount();
    // umsg_Sensors_baro_state_publish(&state_data);

    return ESP_OK;
}

esp_err_t Sensors::setup_therm()
{
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
    _imu.getEvent(&accel, Adafruit_BNO055::adafruit_vector_type_t::VECTOR_ACCELEROMETER);
    _imu.getEvent(&mag, Adafruit_BNO055::adafruit_vector_type_t::VECTOR_MAGNETOMETER);
    _imu.getEvent(&gyro, Adafruit_BNO055::adafruit_vector_type_t::VECTOR_GYROSCOPE);
    _imu.getEvent(&euler_orientation, Adafruit_BNO055::adafruit_vector_type_t::VECTOR_EULER);
    _imu.getEvent(&lin_accel, Adafruit_BNO055::adafruit_vector_type_t::VECTOR_LINEARACCEL);
    _imu.getEvent(&gravity, Adafruit_BNO055::adafruit_vector_type_t::VECTOR_GRAVITY);

    uint32_t measure_tick = xTaskGetTickCount();

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

    data.temperature_c = _imu.getTemp();
    data.measure_tick = measure_tick;

    umsg_Sensors_imu_data_publish(&data);
}

void Sensors::log_baro()
{
    // umsg_Sensors_baro_data_t data;
    // float temp = _bme.readTemperature();
    // float pressure = _bme.readPressure();
    // float humidity = _bme.readHumidity();
    // uint32_t measure_tick = xTaskGetTickCount();
    // data.measure_tick = xTaskGetTickCount();
    // // Standard pressure of sea level: 1013.25
    // // Could be pressure at launch site to estimate hight above ground

    // float gained_alt_m = _bme.readAltitude(_start_press_hpa);
    // float alt_above_sea_m = _bme.readAltitude(1013.25);

    // data.pressure_pa = pressure;
    // data.relative_humidity = humidity;
    // data.temperature_c = temp;
    // data.gained_alt_m = gained_alt_m;
    // data.alt_above_sea_m = alt_above_sea_m;
    // data.measure_tick = measure_tick;

    // umsg_Sensors_baro_data_publish(&data);
}

void Sensors::log_therm()
{
    Thermistors::readThermistors(_internal_thermistor_c, _external_thermistor_c);
    uint32_t measure_tick = xTaskGetTickCount();
    umsg_Sensors_thermistor_data_t internal_data;
    umsg_Sensors_thermistor_data_t external_data;

    internal_data.temperature_c = _internal_thermistor_c;
    internal_data.measure_tick = measure_tick;
    umsg_Sensors_thermistor_data_publish_ch(&internal_data, 0);

    external_data.temperature_c = _external_thermistor_c;
    external_data.measure_tick = measure_tick;
    umsg_Sensors_thermistor_data_publish_ch(&external_data, 1);
}
