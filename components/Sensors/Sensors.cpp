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
    ESP_ERROR_CHECK(setup_bme());
    ESP_ERROR_CHECK(setup_therm());
}

esp_err_t Sensors::setup_imu()
{

    umsg_Sensors_imu_state_t state_data;
    state_data.initializing = 1;
    state_data.state = SENSOR_DISCONNECTED;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_imu_state_publish(&state_data);

    cmd_sub_handle = umsg_CommandCenter_command_subscribe(1, 2);
    _imu = MPU6050();
    while (!_imu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_16G, 104, &Wire))
    {
        printf("Could not find a valid MPU6050 Sensor, check wiring!\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    state_data.state = SENSOR_OK;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_imu_state_publish(&state_data);

    _imu.setIntFreeFallEnabled(false);
    _imu.setIntZeroMotionEnabled(false);
    _imu.setIntMotionEnabled(false);

    _imu.setDLPFMode(MPU6050_DLPF_4);

    _imu.setRange(MPU6050_RANGE_8G);
    _imu.setScale(MPU6050_SCALE_1000DPS);

    umsg_Sensors_imu_configuration_t conf_data;
    conf_data.sample_rate_hz = sample_rate_hz;
    conf_data.accelerometer_range = G_8;
    conf_data.gyroscope_scale = DPS_1000;
    conf_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_imu_configuration_publish(&conf_data);

    _imu.setThreshold(1);
    _pitch = 0;
    _roll = 0;
    _yaw = 0;

    state_data.initializing = 0;
    state_data.logging_data = 1;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_imu_state_publish(&state_data);

    return ESP_OK;
}

esp_err_t Sensors::setup_bme()
{
    umsg_Sensors_baro_state_t state_data;
    state_data.initializing = 1;
    state_data.state = SENSOR_DISCONNECTED;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_baro_state_publish(&state_data);

    _bme = Adafruit_BME280();
    while (_bme.begin((uint8_t)BME280_ADDRESS, &Wire) == false)
    {
        printf("BME280 could not be connected\n");
    }

    state_data.state = SENSOR_OK;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_baro_state_publish(&state_data);

    float start_press_pa = _bme.readPressure(); // pressue in Pa
    _start_press_hpa = start_press_pa / 100.0;  // Convert to hPa
    _start_alt_m = _bme.readAltitude(1013.25);

    umsg_Sensors_baro_configuration_t conf_data;
    conf_data.start_pressure_hpa = _start_press_hpa;
    conf_data.start_alt_m = _start_alt_m;
    conf_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_baro_configuration_publish(&conf_data);

    state_data.initializing = 0;
    state_data.logging_data = 1;
    state_data.measure_tick = xTaskGetTickCount();
    umsg_Sensors_baro_state_publish(&state_data);

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
    log_bme();
    log_therm();
}

void Sensors::log_imu()
{
    Activites a = _imu.readActivites();
    if (!a.isDataReady)
        return;

    Vector accel_norm = _imu.readScaledAccel();
    Vector gyro_norm = _imu.readNormalizeGyro();
    float temperature_c = _imu.readTemperature();
    uint32_t measure_tick = xTaskGetTickCount();

    // Diff between current measure time and prev
    _pitch = 0;
    _roll = 0;
    _yaw = 0;

    umsg_Sensors_imu_data_t data;

    data.accelerometer[0] = accel_norm.XAxis;
    data.accelerometer[1] = accel_norm.YAxis;
    data.accelerometer[2] = accel_norm.ZAxis;

    data.gyroscope[0] = gyro_norm.XAxis;
    data.gyroscope[1] = gyro_norm.YAxis;
    data.gyroscope[2] = gyro_norm.ZAxis;

    data.magnetometer[0] = 0;
    data.magnetometer[1] = 0;
    data.magnetometer[2] = 0;

    data.attitude[0] = 0;
    data.attitude[1] = 0;
    data.attitude[2] = 0;

    data.temperature_c = temperature_c;
    data.measure_tick = measure_tick;

    umsg_Sensors_imu_data_publish(&data);
}

void Sensors::log_bme()
{
    umsg_Sensors_baro_data_t data;
    float temp = _bme.readTemperature();
    float pressure = _bme.readPressure();
    float humidity = _bme.readHumidity();
    uint32_t measure_tick = xTaskGetTickCount();
    data.measure_tick = xTaskGetTickCount();
    // Standard pressure of sea level: 1013.25
    // Could be pressure at launch site to estimate hight above ground

    float gained_alt_m = _bme.readAltitude(_start_press_hpa);
    float alt_above_sea_m = _bme.readAltitude(1013.25);

    data.pressure_pa = pressure;
    data.relative_humidity = humidity;
    data.temperature_c = temp;
    data.gained_alt_m = gained_alt_m;
    data.alt_above_sea_m = alt_above_sea_m;
    data.measure_tick = measure_tick;

    umsg_Sensors_baro_data_publish(&data);
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
