idf_component_register(SRCS "lcd.c" "adc.c" "rtc.c" "led.c" "analog_sws.c" "main.c" "dac.c" "encoder.c" "i2c.c"
                    REQUIRES driver spi_flash esp_common esp_driver_pcnt esp_timer esp_lcd lvgl
                    INCLUDE_DIRS "")