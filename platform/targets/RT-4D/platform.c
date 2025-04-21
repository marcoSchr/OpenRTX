/***************************************************************************
 *   Copyright (C) 2021 - 2024 by Federico Amedeo Izzo IU2NUO,             *
 *                                Niccolò Izzo IU2KIN,                     *
 *                                Frederik Saraci IU2NRO,                  *
 *                                Silvano Seva IU2KWO                      *
 *                                Mathis Schmieder DB9MAT                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <interfaces/platform.h>
#include <peripherals/gpio.h>
#include <hwconfig.h>


static hwInfo_t hwInfo =
{
    .vhf_maxFreq = 174,
    .vhf_minFreq = 136,
    .vhf_band    = 1,
    .uhf_maxFreq = 480,
    .uhf_minFreq = 400,
    .uhf_band    = 1,
    .hw_version  = 0,
    .flags       = 0,
    .name        = "RT-4D"
};

void platform_init()
{
    // Configure GPIOs
    gpio_setMode(GREEN_LED, OUTPUT);
    gpio_setMode(RED_LED,   OUTPUT);
}

void platform_terminate()
{
    /* Shut down LEDs */
    gpio_clearPin(GREEN_LED);
    gpio_clearPin(RED_LED);
}

uint16_t platform_getVbat()
{
   return 0;
}

uint8_t platform_getMicLevel()
{
    return 0;
}

uint8_t platform_getVolumeLevel()
{
    return 0;
}

int8_t platform_getChSelector()
{
    return 0;
}

bool platform_getPttStatus()
{
    return false;
}

bool platform_pwrButtonStatus()
{
    return true;
}

void platform_ledOn(led_t led)
{
    switch(led)
    {
        case RED:
            gpio_setPin(RED_LED);
            break;

        case GREEN:
            gpio_setPin(GREEN_LED);
            break;

        default:
            break;
    }
}

void platform_ledOff(led_t led)
{
    switch(led)
    {
        case RED:
            gpio_clearPin(RED_LED);
            break;

        case GREEN:
            gpio_clearPin(GREEN_LED);
            break;

        default:
            break;
    }
}

void platform_beepStart(uint16_t freq)
{
    /* TODO */
    (void) freq;
}

void platform_beepStop()
{
    /* TODO */
}

const hwInfo_t *platform_getHwInfo()
{
    return &hwInfo;
}
