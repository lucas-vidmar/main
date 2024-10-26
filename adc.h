#pragma once

#include <stdio.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "i2c.h"

#define ADS1115_ADDR 0x48
#define ADC_CONVERSION_TIME 8
#define ADC_CHANNEL_IDUT 0
#define ADC_CHANNEL_TEMP 1
#define ADC_CHANNEL_VDUT 2
#define ADC_MAX_VALUE (1 << 16)
#define ADC_PGA 6.144

void adc_setup(void);
esp_err_t adc_read(uint8_t channel, int16_t* value);
float adc_read_voltage(uint8_t channel);
float adc_read_iDUT(void);
float adc_read_temperature(void);
float adc_read_vDUT(void);