#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <esp_err.h>

#include "i2c.h"

#define MCP79410_ADDR 0x6F // 110 1111
#define MCP79410_FREQ_CRYSTAL 32768 // Hz

// rtc Register Addresses
#define rtc_RTCSEC  0x00
#define rtc_RTCMIN  0x01
#define rtc_RTCHOUR 0x02
#define rtc_RTCWKDAY 0x03
#define rtc_RTCDATE 0x04
#define rtc_RTCMTH  0x05
#define rtc_RTCYEAR 0x06

void rtc_setup(void);
void rtc_SetTime(struct tm *time);
void rtc_GetTime(struct tm *time);