#pragma once

#include "driver/i2c.h"
#include "esp_log.h"
#include "i2c.h"

#define MCP4725_ADDR 0x60           /*!< MCP4725 I2C address */

esp_err_t mcp4725_write(uint16_t);