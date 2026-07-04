#ifndef SOFTSPI_MASTER_H
#define SOFTSPI_MASTER_H

#include <stdint.h>
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 软件 SPI 总线配置
 */
typedef struct {
    int sclk_io_num;   // 时钟引脚
    int mosi_io_num;   // 主机输出引脚
    int miso_io_num;   // 主机输入引脚
} softspi_bus_config_t;

/**
 * @brief 初始化软件 SPI 总线
 */
esp_err_t softspi_bus_initialize(const softspi_bus_config_t *cfg);

/**
 * @brief 释放软件 SPI 总线
 */
esp_err_t softspi_bus_free(void);

/**
 * @brief 全双工 SPI 传输
 *
 * @param cs_pin 片选引脚，-1 表示不控制
 * @param tx_buf 发送缓冲区
 * @param rx_buf 接收缓冲区（可为 NULL）
 * @param len    传输字节数
 */
esp_err_t softspi_transfer(int cs_pin, const uint8_t *tx_buf, uint8_t *rx_buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* SOFTSPI_MASTER_H */
