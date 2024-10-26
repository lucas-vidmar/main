idf_component_register(SRCS "adc.c" "rtc.c" "led.c" "analog_sws.c" "main.c" "dac.c" "encoder.c" "i2c.c" "analog_sws.c" "led.c" "rtc.c"
                    REQUIRES driver spi_flash esp_common esp_driver_pcnt esp_timer
                    INCLUDE_DIRS "")