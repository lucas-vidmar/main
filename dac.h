/**
 * @file dac.h
 * @brief Header file for DAC (Digital-to-Analog Converter) functions.
 *
 * This file contains the declarations of functions used to set up and control
 * the DAC.
 */
#pragma once

#include <esp_log.h>

#include "i2c.h"

#define MCP4725_ADDR 0x60   /*!< MCP4725 I2C address */

#define DAC_BITS 12 /*!< DAC resolution in bits */
#define DAC_RESOLUTION (1 << DAC_BITS) /*!< DAC resolution in bits */
#define DAC_REF_VOLTAGE 4.096   /*!< Reference voltage in volts */
#define DAC_Q (DAC_REF_VOLTAGE / DAC_RESOLUTION)    /*!< Voltage step in V */
#define DAC_QmV (Q * 1000)  /*!< Voltage step in mV */
#define DAC_V_MAX 0.5   /*!< Maximum voltage output in V */
#define DAC_OUTPUT_VOLTAGE_DIVIDER (DAC_V_MAX / DAC_REF_VOLTAGE)    /*!< Output voltage divider */
#define DAC_MAX_DIGITAL_VALUE 83

 /**
    * @brief Sets up the DAC.
    *
    * This function initializes the DAC hardware and prepares it for use.
    *
    * @return
    *     - ESP_OK: Success
    *     - ESP_FAIL: Initialization failed
    */
esp_err_t dac_setup(void);
 
 /**
    * @brief Writes a value to the DAC.
    *
    * This function sends a 16-bit value to the DAC to set its output.
    *
    * @param value The 16-bit value to be written to the DAC.
    *
    * @return
    *     - ESP_OK: Success
    *     - ESP_FAIL: Write operation failed
    */
esp_err_t dac_write(uint16_t value);

 /**
    * @brief Sets the DAC output voltage.
    *
    * This function sets the DAC output to a specified voltage in millivolts.
    *
    * @param voltageInMmV The desired output voltage in millivolts.
    *
    * @return
    *     - ESP_OK: Success
    *     - ESP_FAIL: Voltage setting failed
    */
esp_err_t dac_set_voltage(int voltageInMmV);