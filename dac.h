#pragma once

#include <esp_log.h>

#include "i2c.h"

#define MCP4725_ADDR 0x60           /*!< MCP4725 I2C address */

esp_err_t dac_setup(void);
esp_err_t dac_write(uint16_t);