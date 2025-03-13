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

SPI_BITBANG_DEVICE_DEFINE(display_spi, spi_display_config, NULL)