#include "dac.h"

i2c_master_dev_handle_t dac_handle = NULL;

esp_err_t dac_setup(void)
{
    // Create device handle
    ESP_LOGI("DAC", "Adding DAC device");
    ESP_ERROR_CHECK(i2c_master_add_device(MCP4725_ADDR, &dac_handle));

    // Set DAC to default value (0V)
    ESP_LOGI("DAC", "Setting DAC to default value");
    ESP_ERROR_CHECK(dac_write(0));

    return ESP_OK;
}

esp_err_t dac_write(uint16_t value)
{
    ESP_LOGI("DAC", "Writing value to DAC: %d", value);
    // Write value to DAC
    uint8_t data[2];
    data[0] = (value >> 8) & 0x0F;
    data[1] = value & 0xFF;
    return i2c_masterWrite(dac_handle, data, 2);
}

esp_err_t dac_set_voltage(int voltageInMmV)
{
    // Calculate DAC value
    float value = ((voltageInMmV / 1000.0) / DAC_V_MAX) * DAC_RESOLUTION;
    ESP_LOGI("DAC", "Setting DAC voltage to %d mV", voltageInMmV);
    // Round to nearest integer
    return dac_write((uint16_t)(value + 0.5));
}