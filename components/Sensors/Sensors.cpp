#include "Sensors.h"

Vector Sensors::low_pass_filter(Vector accel, Vector accel_filtered)
{
    Vector filtered = {0, 0, 0};
    filtered.XAxis = (1 - fusion_boundary) * accel.XAxis + (fusion_boundary * accel_filtered.XAxis);
    filtered.YAxis = (1 - fusion_boundary) * accel.YAxis + (fusion_boundary * accel_filtered.YAxis);
    filtered.ZAxis = (1 - fusion_boundary) * accel.ZAxis + (fusion_boundary * accel_filtered.ZAxis);
    return filtered;
}

Vector Sensors::high_pass_filter(Vector gyro, Vector past_gyro, Vector gyro_filtered)
{
    Vector filtered = {0, 0, 0};
    filtered.XAxis = (1 - fusion_boundary) * gyro_filtered.XAxis + (1 - fusion_boundary) * (gyro.XAxis - past_gyro.XAxis);
    filtered.YAxis = (1 - fusion_boundary) * gyro_filtered.YAxis + (1 - fusion_boundary) * (gyro.YAxis - past_gyro.YAxis);
    filtered.ZAxis = (1 - fusion_boundary) * gyro_filtered.ZAxis + (1 - fusion_boundary) * (gyro.ZAxis - past_gyro.ZAxis);
    return filtered;
}

Vector Sensors::fuze_imu_data(Vector accel_filter, Vector gyro_filter, Vector past_fuze)
{
    Vector fuzed = {0, 0, 0};
    fuzed.XAxis = (1 - fusion_boundary) * (past_fuze.XAxis + (gyro_filter.XAxis * (1 / sample_rate_hz))) + fusion_boundary * accel_filter.XAxis;
    fuzed.YAxis = (1 - fusion_boundary) * (past_fuze.YAxis + (gyro_filter.YAxis * (1 / sample_rate_hz))) + fusion_boundary * accel_filter.YAxis;
    fuzed.ZAxis = (1 - fusion_boundary) * (past_fuze.ZAxis + (gyro_filter.ZAxis * (1 / sample_rate_hz))) + fusion_boundary * accel_filter.ZAxis;
    return fuzed;
}

void Sensors::vMainLoop_Task(void *arg)
{
    Sensors sensors = *((Sensors *)arg);
    sensors.setup();
    for (;;)
    {
        sensors.log_data();
        sensors.variable_delay(sensors.sample_rate_hz);
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
    cmd_sub_handle = umsg_CommandCenter_command_subscribe(1, 2);
    _imu = MPU6050();
    while (!_imu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_16G, 104, &Wire))
    {
        printf("Could not find a valid MPU6050 Sensor, check wiring!\n");
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    // Disable interupts
    _imu.setIntFreeFallEnabled(false);
    _imu.setIntZeroMotionEnabled(false);
    _imu.setIntMotionEnabled(false);

    _imu.calibrateGyro();
    _imu.setThreshold(2);
    _imu.setRange(MPU6050_RANGE_8G);
    _imu.setScale(MPU6050_SCALE_1000DPS);
    printf("IMU setup complete\n");

    past_gyro = {0, 0, 0};
    fuzed_imu_data = {0, 0, 0};
    accel_filter = {0, 0, 0};
    gyro_filter = {0, 0, 0};

    fusion_boundary = 0.6;

    _pitch = 0;
    _roll = 0;
    _yaw = 0;
    return ESP_OK;
}

esp_err_t Sensors::setup_bme()
{
    _bme = Adafruit_BME280();
    while (_bme.begin((uint8_t)BME280_ADDRESS, &Wire) == false)
    {
        printf("BME280 could not be connected\n");
    }
    float start_press_pa = _bme.readPressure(); // pressue in Pa
    _start_press_hpa = start_press_pa / 100.0;  // Convert to hPa
    _start_alt_m = _bme.readAltitude(1013.25);
    printf("BME280 setup complete\n");
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
    Vector accel_norm = _imu.readScaledAccel();
    Vector gyro_norm = _imu.readNormalizeGyro();
    float temperature_c = _imu.readTemperature();
    uint32_t measure_tick = xTaskGetTickCount();

    // Diff between current measure time and prev
    float time_step = 1.0 / sample_rate_hz;
    _pitch = _pitch + gyro_norm.YAxis * time_step;
    _roll = _roll + gyro_norm.XAxis * time_step;
    _yaw = _yaw + gyro_norm.ZAxis * time_step;

    accel_filter = low_pass_filter(accel_norm, accel_filter);
    gyro_filter = high_pass_filter(gyro_norm, past_gyro, gyro_filter);
    fuzed_imu_data = fuze_imu_data(accel_filter, gyro_filter, fuzed_imu_data);
    past_gyro = gyro_norm;
    // may need to integrate to get the roll pitch yaw
    umsg_Sensors_imu_data_t *data = new umsg_Sensors_imu_data_t;

    *data = {
        .accelerometer = {accel_filter.XAxis, accel_filter.YAxis, accel_filter.ZAxis},
        .accelerometer_processing = SCALED,
        .gyroscope = {gyro_filter.XAxis, gyro_filter.YAxis, gyro_filter.ZAxis},
        .gyroscope_processing = NORMALIZED,
        .temperature_c = temperature_c,
        .measure_tick = measure_tick,
        .attitude = {fuzed_imu_data.XAxis, fuzed_imu_data.YAxis, fuzed_imu_data.ZAxis}};

    umsg_Sensors_imu_data_publish(data);
}

void Sensors::log_bme()
{
    float temp = _bme.readTemperature();
    float pressure = _bme.readPressure();
    float humidity = _bme.readHumidity();
    uint32_t measure_tick = xTaskGetTickCount();
    // Standard pressure of sea level: 1013.25
    // Could be pressure at launch site to estimate hight above ground

    float gained_alt_m = _bme.readAltitude(_start_press_hpa);
    float alt_above_sea_m = _bme.readAltitude(1013.25);

    if (alt_above_sea_m > 12000)
    {
        // xTaskNotify(_cmd_center, 0x01, eSetBits);
    }

    umsg_Sensors_baro_data_t *data = new umsg_Sensors_baro_data_t;
    *data = {
        .pressure_pa = pressure,
        .relative_humidity = humidity,
        .temperature_c = temp,
        .gained_alt_m = gained_alt_m,
        .alt_above_sea_m = alt_above_sea_m,
        .measure_tick = measure_tick};

    umsg_Sensors_baro_data_publish(data);
}

void Sensors::log_therm()
{
    Thermistors::readThermistors(_internal_thermistor_c, _external_thermistor_c);
    umsg_Sensors_thermistor_data_t *internal_data = new umsg_Sensors_thermistor_data_t;
    umsg_Sensors_thermistor_data_t *external_data = new umsg_Sensors_thermistor_data_t;

    *internal_data = {_internal_thermistor_c};
    *external_data = {_external_thermistor_c};

    umsg_Sensors_thermistor_data_publish_ch(internal_data, 0);
    umsg_Sensors_thermistor_data_publish_ch(external_data, 1);
}
