/**
 * @file analog_sws.h
 * @brief Header file for analog switch setup and control functions.
 *
 * This file contains the declarations of functions used for setting up and controlling
 * various analog switches, voltage DAC, MOSFET input modes, and relays in an electronic load system.
 */
#pragma once

#include <esp_err.h>
#include <driver/gpio.h>
#include <esp_log.h>

#define ANALOG_SW1_ENABLE GPIO_NUM_1 // ADG1219BRJZ-REEL Analog Switch | Analog SW1 Enable - DAC Enable
#define ANALOG_SW4_ENABLE GPIO_NUM_19 // ADG1334BRSZ-REEL Analog Switch | Analog SW4 Enable - Power
#define DUT_ENABLE GPIO_NUM_18 // AHES4191 Relay | DUT Enable

/**
 * @brief Sets up the analog switches.
 *
 * This function initializes and configures the analog switches required for the system.
 *
 * @return
 *    - ESP_OK: Success
 *    - ESP_FAIL: Failure
 */
esp_err_t analog_sws_setup(void);

/**
 * @brief Enables the voltage DAC.
 *
 * This function enables the voltage DAC for operation.
 *
 * @return
 *    - ESP_OK: Success
 *    - ESP_FAIL: Failure
 */
esp_err_t vdac_enable(void);

/**
 * @brief Disables the voltage DAC.
 *
 * This function disables the voltage DAC.
 *
 * @return
 *    - ESP_OK: Success
 *    - ESP_FAIL: Failure
 */
esp_err_t vdac_disable(void);

/**
 * @brief Sets the MOSFET input to constant current (CC) mode.
 *
 * This function configures the power MOSFETs input to operate in constant current mode.
 *
 * @return
 *    - ESP_OK: Success
 *    - ESP_FAIL: Failure
 */
esp_err_t mosfet_input_cc_mode(void);

/**
 * @brief Sets the MOSFET input to constant voltage (CV) mode.
 *
 * This function configures the power MOSFETs input to operate in constant voltage mode.
 *
 * @return
 *    - ESP_OK: Success
 *    - ESP_FAIL: Failure
 */
esp_err_t mosfet_input_cv_mode(void);

/**
 * @brief Gets the power MOSFETs input mode.
 *
 * This function retrieves the current operating mode of the power MOSFETs.
 *
 * @return
 *    - ESP_OK: Success
 *    - ESP_FAIL: Failure
 */
esp_err_t get_mosfet_input_mode(void);

/**
 * @brief Enables the relay for the Device Under Test (DUT).
 *
 * This function enables the relay that connects the DUT to the system.
 *
 * @return
 *    - ESP_OK: Success
 *    - ESP_FAIL: Failure
 */
esp_err_t relay_dut_enable(void);

/**
 * @brief Disables the relay for the Device Under Test (DUT).
 *
 * This function disables the relay that connects the DUT to the system.
 *
 * @return
 *    - ESP_OK: Success
 *    - ESP_FAIL: Failure
 */
esp_err_t relay_dut_disable(void);