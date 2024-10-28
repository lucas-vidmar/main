#include "encoder.h"

int encoder_position = 0;
bool encoder_sw_state = false;

pcnt_unit_handle_t pcnt_unit = NULL;
pcnt_channel_handle_t pcnt_chan_a = NULL;
pcnt_channel_handle_t pcnt_chan_b = NULL;

esp_err_t encoder_init()
{

    /* Switch setup */
    // Configure the IOMUX register for pad ENCODER_SW
    ESP_LOGI("Encoder", "Set direction for ENCODER_SW");
    ESP_ERROR_CHECK(gpio_set_direction(ENCODER_SW, GPIO_MODE_INPUT));

    // Set the switch as pullup input
    ESP_LOGI("Encoder", "Set pull mode for ENCODER_SW");
    ESP_ERROR_CHECK(gpio_set_pull_mode(ENCODER_SW, GPIO_PULLUP_ONLY));

    // Configure ENCODER_SW interrupt
    ESP_LOGI("Encoder", "Configured ENCODER_SW interrupt");
    ESP_ERROR_CHECK(gpio_install_isr_service(0));
    ESP_ERROR_CHECK(gpio_isr_handler_add(ENCODER_SW, encoder_itr_sw, NULL));
    ESP_ERROR_CHECK(gpio_set_intr_type(ENCODER_SW, GPIO_INTR_NEGEDGE));

    /* PCNT setup */
    ESP_LOGI("Encoder", "Install pcnt unit");
    pcnt_unit_config_t unit_config = {
        .high_limit = ENCODER_COUNTER_LIMIT + 1,
        .low_limit = -1,
    };
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

    // Set glitch filter
    ESP_LOGI("Encoder", "Set glitch filter");
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = PULSE_FILTER,
    };
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));

    // Configure PCNT channels, A for CLK and B for DT
    ESP_LOGI("Encoder", "Install pcnt channels");
    pcnt_chan_config_t chan_a_config = {
        .edge_gpio_num = ENCODER_CLK,
        .level_gpio_num = ENCODER_DT,
    };
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_a_config, &pcnt_chan_a));
    pcnt_chan_config_t chan_b_config = {
        .edge_gpio_num = ENCODER_DT,
        .level_gpio_num = ENCODER_CLK,
    };
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_b_config, &pcnt_chan_b));

    // Set level and actions for channels
    ESP_LOGI("Encoder", "Set edge and level actions for pcnt channels");
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_a, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_a, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_b, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_b, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

    // Enable the PCNT unit
    ESP_LOGI("Encoder", "Enable pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_LOGI("Encoder", "Clear pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    ESP_LOGI("Encoder", "Start pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));

    return ESP_OK; // All configurations are successful
}

// Interrupt handler for ENCODER_SW
static void IRAM_ATTR encoder_itr_sw(void *arg)
{
    // Debounce the switch non blocking
    static uint32_t last_interrupt_time_sw = 0;
    uint32_t interrupt_time = xTaskGetTickCountFromISR();
    if (interrupt_time - last_interrupt_time_sw < pdMS_TO_TICKS(DEBOUNCE_TIME)) return; // Ignore the interrupt if it was too soon
    last_interrupt_time_sw = interrupt_time; // Update the last interrupt time

    encoder_sw_state = true;    // Set the switch to pressed
}

int encoder_getPosition()
{
    
    ESP_ERROR_CHECK(pcnt_unit_get_count(pcnt_unit, &encoder_position));
    
    
    // Adjust reading to range
    if (encoder_position > ENCODER_COUNTER_LIMIT)
    {
        encoder_position = ENCODER_COUNTER_LIMIT;
    }
    else if (encoder_position < 0)
    {
        encoder_position = 0;
    }

    return encoder_position / 2;
}

bool encoder_getSwitchState()
{
    return encoder_sw_state;
}

void encoder_resetSwitchState()
{
    encoder_sw_state = false;
}