/***************************************************************************
 *   Copyright (C) 2020 by Federico Amedeo Izzo IU2NUO,                    *
 *                         Niccolò Izzo IU2KIN,                            *
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

#ifndef UI_H
#define UI_H

#include <stdbool.h>
#include <state.h>
#include <keyboard.h>
#include <stdint.h>

/**
 * This function initialises the User Interface, starting the 
 * Finite State Machine describing the user interaction.
 */
void ui_init();

/**
 * This function advances the User Interface FSM, basing on the 
 * current radio state and the keys pressed.
 * @param last_state: A local copy of the previous radio state
 * @param keys: A bitmap containing the currently pressed keys
 */
void ui_updateFSM(state_t last_state, keyboard_t keys);

/**
 * This function redraws the GUI based on the last radio state.
 * @param last_state: A local copy of the previous radio state
 * @return true if a screen refresh is needed after the update
 */
bool ui_updateGUI(state_t last_state);

/**
 * This function writes the OpenRTX splash screen image into the framebuffer.
 */
void ui_drawSplashScreen();

/**
 * This function terminates the User Interface.
 */
void ui_terminate();

#endif /* UI_H */