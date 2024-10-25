#pragma once

#include <esp_err.h>
#include <driver/gpio.h>
#include <esp_log.h>

#define ANALOG_SW1_ENABLE GPIO_NUM_1 // ADG1219BRJZ-REEL Analog Switch | Analog SW1 Enable - DAC Enable
#define ANALOG_SW4_ENABLE GPIO_NUM_19 // ADG1334BRSZ-REEL Analog Switch | Analog SW4 Enable - Power
#define DUT_ENABLE GPIO_NUM_18 // AHES4191 Relay | DUT Enable

esp_err_t analog_sws_setup(void);

esp_err_t vdac_enable(void);
esp_err_t vdac_disable(void);

esp_err_t mosfet_input_cc_mode(void);
esp_err_t mosfet_input_cv_mode(void);
esp_err_t get_mosfet_input_mode(void);

esp_err_t relay_dut_enable(void);
esp_err_t relay_dut_disable(void);