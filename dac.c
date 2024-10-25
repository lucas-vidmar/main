#include "dac.h"

esp_err_t mcp4725_write(uint16_t value)
{
    uint8_t write_buf[3];
    write_buf[0] = (value >> 8) & 0x0F; // Upper data bits (D11.D10.D9.D8)
    write_buf[1] = value & 0xFF;        // Lower data bits (D7.D6.D5.D4.D3.D2.D1.D0)

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MCP4725_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(cmd, write_buf, sizeof(write_buf), true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK) {
        ESP_LOGE("DAC_MCP4725", "Failed to write to MCP4725");
    }
    return ret;
}