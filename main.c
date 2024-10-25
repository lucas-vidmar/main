/* STD Libs */
#include <stdio.h>
#include <inttypes.h>
#include <stdio.h>

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

/* Parts */
#include "encoder.h"
#include "dac.h"
#include "i2c.h"

#define HEARTBEAT_PIN GPIO_NUM_2
#define ANALOG_SW1_ENABLE GPIO_NUM_1
#define ANALOG_SW4_ENABLE GPIO_NUM_19
#define DUT_ENABLE GPIO_NUM_18

void setup(void);
void print_esp_info();

void app_main(void)
{
    printf("***************************\n* Electronic Load Project *\n***************************\n");
    print_esp_info();
    printf("***************************\n");
    esp_log_level_set("*", ESP_LOG_DEBUG); // Set all components to log level VERBOSE

    setup();

    while (1)
    {
        static int last_encoder_position = 0;
        static bool last_encoder_switch_state = false;
        if (last_encoder_position != encoder_getPosition() || last_encoder_switch_state != encoder_getSwitchState())
        {
            last_encoder_position = encoder_getPosition();
            last_encoder_switch_state = encoder_getSwitchState();
            printf("Encoder position: %d\n", encoder_getPosition());
            printf("Encoder switch state: %d\n", encoder_getSwitchState());
            printf("---------\n"); 
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void setup(){

    // Configure the IOMUX register for pad HEARTBEAT_PIN
    esp_rom_gpio_pad_select_gpio(HEARTBEAT_PIN);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(HEARTBEAT_PIN, GPIO_MODE_OUTPUT);

    /* SET Analog switches on*/
    gpio_set_direction(ANALOG_SW1_ENABLE, GPIO_MODE_OUTPUT);
    gpio_set_direction(ANALOG_SW4_ENABLE, GPIO_MODE_OUTPUT);
    gpio_set_direction(DUT_ENABLE, GPIO_MODE_OUTPUT);
    gpio_set_level(ANALOG_SW1_ENABLE, 0);
    gpio_set_level(ANALOG_SW4_ENABLE, 0);
    gpio_set_level(DUT_ENABLE, 0);

    
    // Initialize I2C
    ESP_LOGI("MAIN", "----- Initializing I2C master -----");
    ESP_ERROR_CHECK(i2c_master_init());

    // Initialize DAC
    ESP_LOGI("MAIN", "----- Initializing DAC -----");
    ESP_ERROR_CHECK(dac_setup());

    // Initialize encoder
    ESP_LOGI("MAIN", "----- Initializing encoder -----");
    ESP_ERROR_CHECK(encoder_init());
}

void print_esp_info(){

    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

}