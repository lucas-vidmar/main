#include "lcd.h"

static esp_lcd_panel_handle_t panel_handle = NULL;

void lcd_init(void)
{
    /* Inicializar el bus SPI */
    spi_bus_config_t buscfg = {
        .sclk_io_num = PIN_NUM_CLK,
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = -1, // No utilizado
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 320 * 240 * 2 + 10 // Tamaño máximo de transferencia
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

    /* Configurar el controlador de bajo nivel */
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = PIN_NUM_DC,
        .cs_gpio_num = PIN_NUM_CS,
        .pclk_hz = 40 * 1000 * 1000, // 40MHz
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
        .spi_mode = 0,
        .trans_queue_depth = 10,
        .spi_host_id = SPI2_HOST,
    };
    esp_lcd_panel_io_handle_t io_handle = NULL;
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI2_HOST, &io_config, &io_handle));

    /* Configurar el panel LCD */
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = PIN_NUM_RST,
        .color_space = ESP_LCD_COLOR_SPACE_RGB,
        .bits_per_pixel = 16,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_ili9341(io_handle, &panel_config, &panel_handle));

    /* Inicializar el panel */
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    /* Girar la pantalla si es necesario */
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, false));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, false));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, false));

    /* Encender la retroiluminación */
    gpio_set_direction(PIN_NUM_BCKL, GPIO_MODE_OUTPUT);
    gpio_set_level(PIN_NUM_BCKL, 1);
}

void lcd_display_values(uint8_t dac_voltage, float current, float temperature, float measured_voltage)
{
    char buffer[64];

    // Limpiar la pantalla
    esp_lcd_panel_disp_off(panel_handle, false);
    esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, 320, 240, NULL); // Limpia la pantalla (puede variar según la implementación)
    esp_lcd_panel_disp_off(panel_handle, true);

    // Establecer posición y color del texto
    uint16_t x = 10;
    uint16_t y = 20;
    uint16_t color = 0xFFFF; // Blanco

    // Mostrar el voltaje del DAC
    snprintf(buffer, sizeof(buffer), "DAC Voltage: %d mV", dac_voltage);
    esp_lcd_panel_draw_string(panel_handle, x, y, buffer, color);
    y += 30;

    // Mostrar la corriente
    snprintf(buffer, sizeof(buffer), "Current: %.2f A", current);
    esp_lcd_panel_draw_string(panel_handle, x, y, buffer, color);
    y += 30;

    // Mostrar la temperatura
    snprintf(buffer, sizeof(buffer), "Temperature: %.2f C", temperature);
    esp_lcd_panel_draw_string(panel_handle, x, y, buffer, color);
    y += 30;

    // Mostrar la tensión medida
    snprintf(buffer, sizeof(buffer), "Voltage: %.2f V", measured_voltage);
    esp_lcd_panel_draw_string(panel_handle, x, y, buffer, color);
}
