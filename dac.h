#pragma once

#include <esp_log.h>

#include "i2c.h"

#define MCP4725_ADDR 0x60                               /*!< MCP4725 I2C address */

#define DAC_BITS 12 /*!< DAC resolution in bits */
#define DAC_RESOLUTION (1 << DAC_BITS) /*!< DAC resolution in bits */
#define DAC_REF_VOLTAGE 4.096   /*!< Reference voltage in volts */
#define DAC_Q (DAC_REF_VOLTAGE / DAC_RESOLUTION)    /*!< Voltage step in V */
#define DAC_QmV (Q * 1000)  /*!< Voltage step in mV */
#define DAC_V_MAX 0.5   /*!< Maximum voltage output in V */
#define DAC_OUTPUT_VOLTAGE_DIVIDER (DAC_V_MAX / DAC_REF_VOLTAGE)    /*!< Output voltage divider */

esp_err_t dac_setup(void);
esp_err_t dac_write(uint16_t value);
esp_err_t dac_set_voltage(int voltageInMmV);