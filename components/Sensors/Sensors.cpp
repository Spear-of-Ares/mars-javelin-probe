#include "Sensors.h"

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
    _imu.setRange(MPU6050_RANGE_2G);
    _imu.setScale(MPU6050_SCALE_2000DPS);
    printf("IMU setup complete\n");

    _pitch = 0;
    _roll = 0;
    _yaw = 0;
    return ESP_OK;
}

esp_err_t Sensors::setup_bme()
{
    _bme = Adafruit_BME280();
#ifdef BME_ATTACHED
    while (_bme.begin((uint8_t)BME280_ADDRESS, &Wire) == false)
    {
        printf("BME280 could not be connected\n");
    }
#endif
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
    // accel_norm.XAxis = accel_norm.XAxis/3;
    // accel_norm.YAxis = accel_norm.YAxis/3;
    // accel_norm.ZAxis = accel_norm.ZAxis/3;

    Vector gyro_norm = _imu.readNormalizeGyro();
    float temperature_c = _imu.readTemperature();

    // Diff between current measure time and prev
    float time_step = 1.0 / sample_rate_hz;
    _pitch = _pitch + gyro_norm.YAxis * time_step;
    _roll = _roll + gyro_norm.XAxis * time_step;
    _yaw = _yaw + gyro_norm.ZAxis * time_step;

    // Cannot use modulus as p,r,y are float values
    while (_pitch > 180)
    {
        _pitch = _pitch - 360;
    }
    while (_pitch < -180)
    {
        _pitch = _pitch + 360;
    }

    while (_roll > 180)
    {
        _roll = _roll - 360;
    }
    while (_roll < 180)
    {
        _roll = _roll + 360;
    }

    while (_yaw > 180)
    {
        _yaw = _yaw - 360;
    }
    while (_yaw < 180)
    {
        _yaw = _yaw + 360;
    }

    // Store measure time
}

void Sensors::log_bme()
{
    float temp = _bme.readTemperature();
    float pressure = _bme.readPressure();
    float humidity = _bme.readHumidity();
    // Standard pressure of sea level: 1013.25
    // Could be pressure at launch site to estimate hight above ground

    float alt_inc_m = _bme.readAltitude(_start_press_hpa);
    float alt_asl_m = _bme.readAltitude(1013.25);

    if (alt_asl_m > 12000)
    {
        // xTaskNotify(_cmd_center, 0x01, eSetBits);
    }
}

void Sensors::log_therm()
{
    Thermistors::readThermistors(_internal_thermistor_c, _external_thermistor_c);
}