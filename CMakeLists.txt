idf_component_register(SRCS "main.c" "dac.c" "encoder.c" "i2c.c"
                    REQUIRES driver spi_flash esp_common esp_driver_pcnt
                    INCLUDE_DIRS "")