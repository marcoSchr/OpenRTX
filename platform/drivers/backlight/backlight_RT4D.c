/***************************************************************************
 *   Copyright (C) 2025 by Marco Schröder DM4RCO,                          *
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

#include <hwconfig.h>
#include <interfaces/platform.h>
#include <peripherals/gpio.h>
#include "backlight.h"

void backlight_init()
{
    gpio_setMode(LCD_BACKLIGHT, ALTERNATE);
    GPIOA->muxl_bit.muxl3 = GPIO_MUX_3;
    CRM->apb2en_bit.tmr9en = TRUE;
    CRM->ahben1_bit.gpioaen = TRUE;

    uint32_t frequency = 200000;
    uint32_t period = ((144000000 / 2) / frequency ) - 1;

    TMR9->pr_bit.pr = period;
    TMR9->div_bit.div = 0x02;
    TMR9->swevt_bit.ovfswtr = TRUE;
    TMR9->ctrl1_bit.clkdiv = TMR_CLOCK_DIV1;
    TMR9->cm1_output_bit.c2octrl = TMR_OUTPUT_CONTROL_PWM_MODE_A;
    TMR9->cctrl_bit.c2p = TMR_OUTPUT_ACTIVE_HIGH;
    TMR9->cctrl_bit.c2en = TRUE; // Enable channel
    TMR9->c2dt_bit.c2dt = (50 * (period - 1)) / 100; // Set duty cycles
    TMR9->cm1_output_bit.c2oben = TRUE;
    TMR9->ctrl1_bit.prben = TRUE;
    TMR9->ctrl1_bit.tmren = TRUE; // Start timer
    TMR9->brk_bit.oen = TRUE; // Enable timer
}

void backlight_terminate()
{
    gpio_clearPin(LCD_BACKLIGHT);
}

/*
 * This function is defined in display.h
 */
void display_setBacklightLevel(uint8_t level)
{
    if(level > 100) level = 100;
    uint32_t frequency = 200000;
    uint32_t period = ((144000000 / 2) / frequency ) - 1;
    TMR9->c2dt_bit.c2dt = (level * (period - 1)) / 100; // Set duty cycles
}