#include "i2c.h"

#define DATA_LENGTH 100

i2c_master_bus_handle_t i2c_bus_handle = NULL; // I2C bus handle

esp_err_t i2c_master_init(void)
{
    // Configure I2C master
    ESP_LOGI("I2C", "Initializing I2C master");
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7, // 7 glitches are ignored by default
        .flags.enable_internal_pullup = true,
    };
    return i2c_new_master_bus(&i2c_mst_config, &i2c_bus_handle);
}

esp_err_t i2c_master_add_device(uint8_t addr, i2c_master_dev_handle_t* dev_handle)
{
    ESP_LOGI("I2C", "Adding I2C device at address 0x%02X", addr);
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = addr,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };
    return i2c_master_bus_add_device(i2c_bus_handle, &dev_cfg, dev_handle);
}

esp_err_t i2c_masterWrite(i2c_master_dev_handle_t dev_handle, uint8_t *data_wr, size_t size)
{
    ESP_LOGI("I2C", "Writing the following data to I2C device: %s", data_wr);
    return i2c_master_transmit(dev_handle, data_wr, size, I2C_TIMEOUT);
}

esp_err_t i2c_masterRead(i2c_master_dev_handle_t dev_handle, uint8_t *data_rd, size_t size)
{
    ESP_LOGI("I2C", "Writing data to I2C device");
    return i2c_master_receive(dev_handle, data_rd, size, I2C_TIMEOUT);
}