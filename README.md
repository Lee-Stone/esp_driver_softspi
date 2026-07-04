# ESP SoftSPI Master

基于 GPIO 位带操作的软件 SPI 主机驱动，用于 ESP32 系列芯片。API 命名风格对齐 ESP-IDF 官方 `spi_master` 驱动。

当硬件 SPI 主机（SPI2/SPI3）资源不足时，可使用本组件通过任意 GPIO 引脚模拟 SPI 通信。

## API 对照

| 软件 SPI（本组件） | 硬件 SPI（官方 spi_master） |
|---|---|
| `softspi_bus_config_t` | `spi_bus_config_t` |
| `softspi_bus_initialize()` | `spi_bus_initialize()` |
| `softspi_bus_free()` | `spi_bus_free()` |
| `softspi_transfer()` | `spi_device_transmit()` |

## 添加组件到工程

```bash
idf.py add-dependency https://github.com/Lee-Stone/esp_driver_softspi.git
```

或手动编辑 `main/idf_component.yml`：

```yaml
dependencies:
  lee-stone/esp_driver_softspi:
    git: https://github.com/Lee-Stone/esp_driver_softspi.git
```

## 示例用法

### 初始化

```c
#include "softspi_master.h"

softspi_bus_config_t bus_cfg = {
    .sclk_io_num = 15,   // 时钟引脚
    .mosi_io_num = 6,    // 主机输出引脚
    .miso_io_num = 5,    // 主机输入引脚
};
ESP_ERROR_CHECK(softspi_bus_initialize(&bus_cfg));
```

### 全双工传输

```c
// 发送 1 字节命令，读取 2 字节数据
uint8_t tx[] = { 0x90 };    // 命令字节
uint8_t rx[3];              // 接收缓冲区（比发送多 1 字节）
softspi_transfer(7, tx, rx, 3);  // CS = GPIO7

// rx[0] 为发送命令时收到的垃圾字节
// rx[1]、rx[2] 为有效数据
```

### 释放总线

```c
ESP_ERROR_CHECK(softspi_bus_free());
```

## 注意事项

- 仅支持主机（Master）模式，最多 8 位数据位，MSB 优先
- 时钟频率由 GPIO 翻转速度决定，ESP32-S3 上实测约 2~4 MHz
- 不支持 DMA 和中断，所有传输均为阻塞式
- 当前仅支持单总线实例（同一时刻只能初始化一条软件 SPI 总线）
- CS 引脚由用户自行管理，通过 `softspi_transfer` 的 `cs_pin` 参数控制
