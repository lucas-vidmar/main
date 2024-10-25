#include "rtc.h"

i2c_master_dev_handle_t rtc_handle = NULL;
struct tm rtc_time = {
    .tm_sec = 0,
    .tm_min = 0,
    .tm_hour = 0,
    .tm_wday = 0,
    .tm_mday = 0,
    .tm_mon = 0,
    .tm_year = 0
};

void rtc_setup() {
    // Create device handle
    ESP_LOGI("RTC", "Adding RTC device");
    ESP_ERROR_CHECK(i2c_master_add_device(MCP79410_ADDR, &rtc_handle));
}

void rtc_SetTime(struct tm *time) {
    uint8_t data[7];

    ESP_LOGI("RTC", "Setting time on RTC");
    data[0] = (time->tm_sec % 10) | ((time->tm_sec / 10) << 4);
    data[1] = (time->tm_min % 10) | ((time->tm_min / 10) << 4);
    data[2] = (time->tm_hour % 10) | ((time->tm_hour / 10) << 4);
    data[3] = (time->tm_wday & 0x07);
    data[4] = (time->tm_mday % 10) | ((time->tm_mday / 10) << 4);
    data[5] = ((time->tm_mon + 1) % 10) | (((time->tm_mon + 1) / 10) << 4);
    data[6] = (time->tm_year % 10) | (((time->tm_year / 10) % 10) << 4);

    ESP_ERROR_CHECK(i2c_masterWrite(rtc_handle, data, 7));
}

void rtc_GetTime(struct tm *time) {
    uint8_t data[7];

    ESP_LOGI("RTC", "Reading time from RTC");
    ESP_ERROR_CHECK(i2c_masterRead(rtc_handle, data, 7));

    time->tm_sec = (data[0] & 0x0F) + ((data[0] >> 4) * 10);
    time->tm_min = (data[1] & 0x0F) + ((data[1] >> 4) * 10);
    time->tm_hour = (data[2] & 0x0F) + ((data[2] >> 4) * 10);
    time->tm_wday = data[3] & 0x07;
    time->tm_mday = (data[4] & 0x0F) + ((data[4] >> 4) * 10);
    time->tm_mon = ((data[5] & 0x0F) + ((data[5] >> 4) * 10)) - 1;
    time->tm_year = (data[6] & 0x0F) + ((data[6] >> 4) * 10) + 100;
}