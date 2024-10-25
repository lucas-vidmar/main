#include "led.h"

esp_timer_handle_t heartbeat_timer = NULL;

void led_setup(void) {
    // Configure the IOMUX register for pad LED_PIN
    ESP_LOGI("LED", "Configuring LED");
    ESP_ERROR_CHECK(gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT));

    // Create heartbeat timer
    ESP_LOGI("LED", "Creating heartbeat timer");
    const esp_timer_create_args_t heartbeat_timer_args = {
        .callback = &heartbeat_timer_callback,
        .name = "heartbeat_timer"
    };
    ESP_ERROR_CHECK(esp_timer_create(&heartbeat_timer_args, &heartbeat_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(heartbeat_timer, HEARTBEAT_DELAY_MS * 1000));
}

static void IRAM_ATTR heartbeat_timer_callback(void* arg) {
    static bool led_on = false;
    gpio_set_level(LED_PIN, led_on);
    led_on = !led_on;
}