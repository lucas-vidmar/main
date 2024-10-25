#include "encoder.h"

int encoder_position = 0;
bool encoder_sw_state = false;

esp_err_t encoder_init()
{

    /* Switch setup */
    // Configure the IOMUX register for pad ENCODER_SW
    ESP_ERROR_CHECK(gpio_set_direction(ENCODER_SW, GPIO_MODE_INPUT));
    ESP_LOGI("Encoder", "Set direction for ENCODER_SW");

    // Set the switch as pullup input
    ESP_ERROR_CHECK(gpio_set_pull_mode(ENCODER_SW, GPIO_PULLUP_ONLY));
    ESP_LOGI("Encoder", "Set pull mode for ENCODER_SW");

    // Configure ENCODER_SW interrupt
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_isr_handler_add(ENCODER_SW, encoder_itr_sw, NULL));
    ESP_ERROR_CHECK(gpio_set_intr_type(ENCODER_SW, GPIO_INTR_NEGEDGE));
    ESP_LOGI("Encoder", "Configured ENCODER_SW interrupt");

    /* Encoder setup */
    // Configure the IOMUX register for pad ENCODER_CLK
    ESP_ERROR_CHECK(gpio_set_direction(ENCODER_CLK, GPIO_MODE_INPUT));
    ESP_LOGI("Encoder", "Set direction for ENCODER_CLK");

    // Configure the IOMUX register for pad ENCODER_DT
    ESP_ERROR_CHECK(gpio_set_direction(ENCODER_DT, GPIO_MODE_INPUT));
    ESP_LOGI("Encoder", "Set direction for ENCODER_DT");
    
    // Set the encoder as floating input
    ESP_ERROR_CHECK(gpio_set_pull_mode(ENCODER_CLK, GPIO_FLOATING));
    ESP_ERROR_CHECK(gpio_set_pull_mode(ENCODER_DT, GPIO_FLOATING));
    ESP_LOGI("Encoder", "Set pull mode for ENCODER_CLK and ENCODER_DT");

    // Initialize PCNT unit
    pcnt_config_t pcnt_config = {
        .pulse_gpio_num = ENCODER_CLK,
        .ctrl_gpio_num = ENCODER_DT,
        .channel = PCNT_CHANNEL_0,
        .unit = PCNT_UNIT_0,
        .pos_mode = PCNT_COUNT_DEC,   // Count down on the positive edge
        .neg_mode = PCNT_COUNT_INC,   // Count up on the negative edge
        .lctrl_mode = PCNT_MODE_REVERSE, // Reverse counting direction if low
        .hctrl_mode = PCNT_MODE_KEEP, // Keep the primary counter mode if high
        .counter_h_lim = INT16_MAX,
        .counter_l_lim = INT16_MIN,
    };

    // Configure PCNT unit
    ESP_ERROR_CHECK(pcnt_unit_config(&pcnt_config));
    ESP_LOGI("Encoder", "Configured PCNT unit");

    // Add filter to PCNT input signals
    ESP_ERROR_CHECK(pcnt_set_filter_value(PCNT_UNIT_0, PULSE_FILTER));
    ESP_ERROR_CHECK(pcnt_filter_enable(PCNT_UNIT_0));
    ESP_LOGI("Encoder", "Added filter to PCNT input signals");

    // Pause PCNT's counter first
    ESP_ERROR_CHECK(pcnt_counter_pause(PCNT_UNIT_0));
    ESP_LOGI("Encoder", "Paused PCNT counter");

    // Reset counter value
    ESP_ERROR_CHECK(pcnt_counter_clear(PCNT_UNIT_0));
    ESP_LOGI("Encoder", "Cleared PCNT counter");

    // Enable PCNT event in high limit
    ESP_ERROR_CHECK(pcnt_event_enable(PCNT_UNIT_0, PCNT_EVT_H_LIM));
    ESP_LOGI("Encoder", "Enabled PCNT events");

    // Enable PCNT event in low limit
    ESP_ERROR_CHECK(pcnt_event_enable(PCNT_UNIT_0, PCNT_EVT_L_LIM));
    ESP_LOGI("Encoder", "Enabled PCNT events");

    // Start PCNT unit
    ESP_ERROR_CHECK(pcnt_counter_resume(PCNT_UNIT_0));
    ESP_LOGI("Encoder", "Resumed PCNT counter");

    return ESP_OK; // All configurations are successful
}

// Interrupt handler for ENCODER_SW
static void IRAM_ATTR encoder_itr_sw(void *arg)
{
    // Debounce the switch non blocking
    static uint32_t last_interrupt_time_sw = 0;
    uint32_t interrupt_time = xTaskGetTickCountFromISR();
    if (interrupt_time - last_interrupt_time_sw < pdMS_TO_TICKS(DEBOUNCE_TIME))
    { // debounce
        return;
    }
    last_interrupt_time_sw = interrupt_time; // Update the last interrupt time
    encoder_sw_state = !encoder_sw_state;    // Toggle the switch state
}

int encoder_getPosition()
{
    int16_t count;
    pcnt_get_counter_value(PCNT_UNIT_0, &count);
    return count;
}

bool encoder_getSwitchState()
{
    return encoder_sw_state;
}
