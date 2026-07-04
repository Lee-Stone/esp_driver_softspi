#include <string.h>
#include "softspi_master.h"
#include "driver/gpio.h"

static int s_clk  = -1;
static int s_mosi = -1;
static int s_miso = -1;

esp_err_t softspi_bus_initialize(const softspi_bus_config_t *cfg)
{
    if (!cfg) return ESP_ERR_INVALID_ARG;

    s_clk  = cfg->sclk_io_num;
    s_mosi = cfg->mosi_io_num;
    s_miso = cfg->miso_io_num;

    gpio_config_t out_conf = {
        .pin_bit_mask = (1ULL << s_clk) | (1ULL << s_mosi),
        .mode         = GPIO_MODE_OUTPUT,
        .pull_up_en   = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&out_conf);

    gpio_config_t in_conf = {
        .pin_bit_mask = (1ULL << s_miso),
        .mode         = GPIO_MODE_INPUT,
        .pull_up_en   = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type    = GPIO_INTR_DISABLE,
    };
    gpio_config(&in_conf);

    gpio_set_level(s_clk, 0);
    return ESP_OK;
}

esp_err_t softspi_bus_free(void)
{
    if (s_clk >= 0)  gpio_reset_pin(s_clk);
    if (s_mosi >= 0) gpio_reset_pin(s_mosi);
    if (s_miso >= 0) gpio_reset_pin(s_miso);
    s_clk = s_mosi = s_miso = -1;
    return ESP_OK;
}

esp_err_t softspi_transfer(int cs_pin, const uint8_t *tx_buf, uint8_t *rx_buf, size_t len)
{
    if (!tx_buf || len == 0) return ESP_ERR_INVALID_ARG;

    int clk = s_clk, mosi = s_mosi, miso = s_miso;

    if (cs_pin >= 0) gpio_set_level(cs_pin, 0);

    for (size_t n = 0; n < len; n++) {
        uint8_t tx = tx_buf[n];
        uint8_t rx = 0;
        for (int i = 7; i >= 0; i--) {
            gpio_set_level(clk, 0);
            gpio_set_level(mosi, (tx >> i) & 1);
            if (rx_buf) rx = (rx << 1) | gpio_get_level(miso);
            gpio_set_level(clk, 1);
        }
        if (rx_buf) rx_buf[n] = rx;
    }

    if (cs_pin >= 0) gpio_set_level(cs_pin, 1);

    return ESP_OK;
}
