#include "analog_sws.h"
#include <driver/uart.h>

esp_err_t analog_sws_setup(void)
{
    // Configure the IOMUX register for analog switches
    ESP_LOGI("ANALOG SWS", "Configuring analog switches");
    //uart_driver_delete(UART_NUM_0);
    //gpio_reset_pin(ANALOG_SW1_ENABLE);
    //esp_rom_gpio_pad_select_gpio(ANALOG_SW1_ENABLE);
    ESP_ERROR_CHECK(gpio_set_direction(ANALOG_SW1_ENABLE, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(ANALOG_SW4_ENABLE, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_direction(DUT_ENABLE, GPIO_MODE_OUTPUT));

    return ESP_OK;
}

esp_err_t vdac_enable(void) { return gpio_set_level(ANALOG_SW1_ENABLE, 1); }

esp_err_t vdac_disable(void) { return gpio_set_level(ANALOG_SW1_ENABLE, 0); }

esp_err_t mosfet_input_cc_mode(void) { return gpio_set_level(ANALOG_SW4_ENABLE, 1); }

esp_err_t mosfet_input_cv_mode(void) { return gpio_set_level(ANALOG_SW4_ENABLE, 0); }

esp_err_t get_mosfet_input_mode(void) { return gpio_get_level(ANALOG_SW4_ENABLE); }

esp_err_t relay_dut_enable(void) { return gpio_set_level(DUT_ENABLE, 1); }

esp_err_t relay_dut_disable(void) { return gpio_set_level(DUT_ENABLE, 0); }