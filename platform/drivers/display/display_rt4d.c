//
// Created by marco on 3/2/25.
//

#include <interfaces/platform.h>
#include <peripherals/gpio.h>
#include <hwconfig.h>
#include <interfaces/delays.h>
#include <interfaces/display.h>
#include "spi_bitbang.h"
#include "at32f423.h"
#include "at32f423_gpio.h"

const struct spiConfig spi_display_config =
{
    .clk       = { LCD_CLK  },
    .mosi      = { LCD_DAT },
    .miso      = { LCD_DAT },
    .clkPeriod = SCK_PERIOD_FROM_FREQ(1000000),
    .flags     = SPI_HALF_DUPLEX
  };
SPI_BITBANG_DEVICE_DEFINE(display_spi, spi_display_config, NULL)

void display_init()
{
    gpio_setMode(LCD_BACKLIGHT, OUTPUT);
    gpio_setMode(LCD_CD,  OUTPUT);
    gpio_setMode(LCD_CS,  OUTPUT);
    gpio_setMode(LCD_CLK, OUTPUT);
    // GPIOB->muxl_bit.muxl3 = GPIO_MUX_6;
    // GPIOB->pull_bit.pull3 = GPIO_PULL_DOWN;
    // GPIOB->odrvr_bit.odrv3 = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_setMode(LCD_DAT, OUTPUT);
    // GPIOB->muxl_bit.muxl5 = GPIO_MUX_6;
    // GPIOB->pull_bit.pull5 = GPIO_PULL_UP;
    // GPIOB->odrvr_bit.odrv3 = GPIO_DRIVE_STRENGTH_STRONGER;
    gpio_setMode(LCD_RST, OUTPUT);
    // Display power on
    gpio_setPin(LCD_BACKLIGHT);

    gpio_setPin(LCD_CS);
    gpio_clearPin(LCD_CD);

    gpio_clearPin(LCD_RST);     /* Reset controller                          */
    delayMs(1);
    gpio_setPin(LCD_RST);
    delayMs(5);

    uint8_t initData[] =
    {
        0xe2,  /* Reset */
        0x2c,
        0x2e,  /*  Regulation resistor ratio */
        0x2f,  /* Voltage Follower On            */
        0x25,  /* Regulation resistor ratio */
        0x81,  /* Set Electronic Volume          */
        0x15,   /* Contrast, initial setting      */
        0xa2,  /* Set Bias = 1/9                 */
        0xc0,  /* Set COM Direction              */
        0xa0,  /* A0 Set SEG Direction           */
        0x40,  /* Set Start Address */
        0xb0,  /* Set Page Address*/
        0xa6,  /* Inverse Display */
        0xaf   /* Set Display Enable             */
    };

    gpio_clearPin(LCD_CS);
    gpio_clearPin(LCD_CD);      /* RS low -> command mode   */
    spi_send(&display_spi, initData, sizeof(initData));
    gpio_clearPin(LCD_CS);
}

void display_terminate()
{

}

static void display_renderRow(uint8_t row, uint8_t *frameBuffer)
{
    /* magic stuff */
    uint8_t *buf = (frameBuffer + 128 * row);
    for (uint8_t i = 0; i<16; i++)
    {
        uint8_t tmp[8] = {0};
        for (uint8_t j = 0; j < 8; j++)
        {
            uint8_t tmp_buf = buf[j*16 + i];
            int count = __builtin_popcount(tmp_buf);
            while (count > 0)
            {
                int pos = __builtin_ctz(tmp_buf);
                tmp[pos] |= 1UL << j;
                tmp_buf &= ~(1 << pos);
                count--;
            }
        }

        spi_send(&display_spi, tmp, 8);
    }
}

void display_renderRows(uint8_t startRow, uint8_t endRow, void *fb)
{
    gpio_clearPin(LCD_CS);

    for(uint8_t row = startRow; row < endRow; row++)
    {
        uint8_t command[3];
        command[0] = 0xB0 | row; /* Set Y position */
        command[1] = 0x10;       /* Set X position */
        command[2] = 0x04;

        gpio_clearPin(LCD_CD);            /* RS low -> command mode */
        spi_send(&display_spi, command, 3);
        gpio_setPin(LCD_CD);              /* RS high -> data mode   */
        display_renderRow(row, (uint8_t *) fb);
    }

    gpio_setPin(LCD_CS);
}

void display_render(void *fb)
{
    display_renderRows(0, CONFIG_SCREEN_HEIGHT / 8, fb);
}

void display_setContrast(uint8_t contrast)
{
    uint8_t command[2];
    command[0] = 0x81;             /* Set Electronic Volume               */
    command[1] = contrast >> 2;    /* Controller contrast range is 0 - 63 */


    gpio_clearPin(LCD_CS);

    gpio_clearPin(LCD_CD);          /* RS low -> command mode              */
    spi_send(&display_spi, command, 2);
    gpio_setPin(LCD_CS);
}


void display_setBacklightLevel(uint8_t level)
{
    (void) level;
}
