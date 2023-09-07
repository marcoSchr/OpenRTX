/***************************************************************************
 *   Copyright (C) 2020 - 2023 by Federico Amedeo Izzo IU2NUO,             *
 *                                Niccolò Izzo IU2KIN                      *
 *                                Frederik Saraci IU2NRO                   *
 *                                Silvano Seva IU2KWO                      *
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

#include <zephyr/input/input.h>
#include <interfaces/keyboard.h>
#include <interfaces/platform.h>
#include <hwconfig.h>
#include <pmu.h>

static const struct device *const buttons_dev = DEVICE_DT_GET(DT_NODELABEL(buttons));

static int8_t  old_pos = 0;
static keyboard_t keys = 0;


static void gpio_keys_cb_handler(struct input_event *evt)
{
    uint32_t keyCode = 0;

    // Map betweek Zephyr keys and OpenRTX keys
    switch(evt->code)
    {
        case INPUT_KEY_VOLUMEDOWN:
            keyCode = KEY_MONI;
            break;

        case INPUT_BTN_START:
            keyCode = KEY_ENTER;
            break;

        case INPUT_BTN_SELECT:
            keyCode = KEY_ESC;
            break;
    }

    if(evt->value != 0)
        keys |= keyCode;
    else
        keys &= ~keyCode;
}

INPUT_CALLBACK_DEFINE(buttons_dev, gpio_keys_cb_handler);


void kbd_init()
{
    // Initialise old position
    old_pos = platform_getChSelector();
}

void kbd_terminate()
{
}

keyboard_t kbd_getKeys()
{
    keys &= ~KNOB_LEFT;
    keys &= ~KNOB_RIGHT;

    // Read rotary encoder to send KNOB_LEFT and KNOB_RIGHT events
    int8_t new_pos = platform_getChSelector();
    if (old_pos != new_pos)
    {
        int8_t delta = new_pos - old_pos;

        // Normal case: handle up/down by looking at the pulse difference
        if(delta > 0)
            keys |= KNOB_LEFT;
        else if (delta < 0)
            keys |= KNOB_RIGHT;

        // Corner case 1: rollover from negative (old) to positive (new) value.
        // Delta is positive but it counts as a down key.
        if((old_pos < 0) && (new_pos > 0))
        {
            keys &= ~KNOB_LEFT;
            keys |= KNOB_RIGHT;
        }

        // Corner case 2: rollover from positive (old) to negative (new) value.
        // Delta is negative but it counts as an up key.
        if((old_pos > 0) && (new_pos < 0))
        {
            keys &= ~KNOB_RIGHT;
            keys |= KNOB_LEFT;
        }

        old_pos = new_pos;
    }

    return keys;
}