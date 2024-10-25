idf_component_register(SRCS "analog_sws.c" "main.c" "dac.c" "encoder.c" "i2c.c" "analog_sws.c"
                    REQUIRES driver spi_flash esp_common esp_driver_pcnt
                    INCLUDE_DIRS "")