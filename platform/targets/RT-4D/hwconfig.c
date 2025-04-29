//
// Created by marco on 3/20/25.
//

#include <spi_bitbang.h>
#include <hwconfig.h>

const struct spiConfig spi_display_config =
{
    .clk       = { LCD_CLK  },
    .mosi      = { LCD_DAT },
    .miso      = { LCD_DAT },
    .clkPeriod = SCK_PERIOD_FROM_FREQ(1000000),
    .flags     = SPI_HALF_DUPLEX
};
const struct spiConfig spi_flash_config =
{
    .clk       = { EFLASH_SCK  },
    .mosi      = { EFLASH_MOSI },
    .miso      = { EFLASH_MISO },
    .clkPeriod = SCK_PERIOD_FROM_FREQ(1000000),
    .flags     = 0
};

SPI_BITBANG_DEVICE_DEFINE(display_spi, spi_display_config, NULL)
SPI_BITBANG_DEVICE_DEFINE(flash_spi, spi_flash_config, NULL)
