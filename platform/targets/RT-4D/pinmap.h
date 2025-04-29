/***************************************************************************
 *   Copyright (C) 2023 by Federico Amedeo Izzo IU2NUO,                    *
 *                         Niccolò Izzo IU2KIN,                            *
 *                         Frederik Saraci IU2NRO,                         *
 *                         Silvano Seva IU2KWO                             *
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

#ifndef PINMAP_H
#define PINMAP_H

#include <at32f423.h>

// LCD Display
#define LCD_CLK          GPIOB,3
#define LCD_DAT          GPIOB,5
#define LCD_RST          GPIOF,6
#define LCD_CS           GPIOA,15
#define LCD_CD           GPIOB,4
#define LCD_BACKLIGHT    GPIOA,3

// LEDs
#define RED_LED          GPIOA,13
#define GREEN_LED        GPIOA,14

// Analog inputs
#define AIN_VBAT         GPIOB,2

// Push-to-talk
#define PTT_SW           GPIOA,12 // Shared with UART RX

// Keyboard and side keys
#define KBD_ROW1         GPIOF,1
#define KBD_ROW2         GPIOA,8
#define KBD_ROW3         GPIOA,9
#define KBD_ROW4         GPIOB,11
#define KBD_COL1         GPIOA,0
#define KBD_COL2         GPIOA,1
#define KBD_COL3         GPIOF,0
#define KBD_COL4         GPIOB,0

// External Flash
#define EFLASH_MISO GPIOB,14
#define EFLASH_MOSI GPIOB,15
#define EFLASH_SCK  GPIOB,13
#define EFLASH_CS   &GpioB,12

// BK1080
#define BK1080_SCK  GPIOC,13
#define BK1080_SDA  GPIOC,14

// BK4819
#define BK4819_CLK  GPIOB,9
#define BK4819_DAT  GPIOB,7
#define BK4819_CS   GPIOB,8

#define USART6_TX   GPIOA,11
#define USART6_RX   GPIOA,12  // Shared with PTT

#endif /* PINMAP_H */