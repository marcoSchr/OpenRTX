/***************************************************************************
*   Copyright (C) 2025 by Marco Schröder DM4RCO                            *
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

#ifndef CPS_DATA_RT4D_H
#define CPS_DATA_RT4D_H

#include <stdint.h>

typedef struct
{
    uint8_t  bandwidth;
    uint16_t rx_code: 12,
             rx_code_type: 4;
    uint32_t rx_frequency;
    uint32_t tx_frequency;
    uint16_t tx_code: 12,
             tx_code_type: 4;
    uint8_t  tx_power;
    uint8_t  tx_priority;
    uint8_t  tx_timeout: 5,
             dcs_encryption: 3;
    uint8_t  scramble: 4,
             tail_tone: 3,
             scan_list: 1;
    uint32_t mute_code_0;
    uint32_t mute_code_1;
    uint32_t mute_code_2;
}
__attribute__((packed))RT4DAnalogChannel;

typedef struct
{
    uint8_t  timeslot;
    uint8_t  color_code;
    uint8_t  dcdm;
    uint32_t rx_frequency;
    uint32_t tx_frequency;
    uint8_t  promiscuous_mode;
    uint8_t  _unused0;
    uint8_t  tx_power;
    uint8_t  tx_priority;
    uint8_t  _unused1;
    uint8_t  _unused2: 7,
             scan_list: 1;
    uint8_t  tx_timeout: 5,
             _unused3: 3;
    uint8_t  _unused4;
    uint16_t rc_group;
    uint16_t contact;
    uint16_t key;
    uint32_t id;
}
__attribute__((packed))RT4DDigitalChannel;


typedef struct
{
    uint8_t flags;
    uint8_t _unused1;
    uint8_t channel_type;
    union
    {
        RT4DAnalogChannel analog;
        RT4DDigitalChannel digital;
    } channel_data;
    uint8_t name[16];
}
__attribute__((packed)) RT4DChannel_t;

#endif //CPS_DATA_RT4D_H
