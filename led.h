#pragma once

#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_timer.h>
#include <esp_err.h>
#include <esp_log.h>

#define LED_PIN GPIO_NUM_2
#define HEARTBEAT_DELAY_MS 1000

void led_setup(void);
static void heartbeat_timer_callback(void* arg);