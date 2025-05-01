/***************************************************************************
 *   Copyright (C) 2025 by Marco Schröder DM4RCO                           *
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

#include <interfaces/nvmem.h>
#include <interfaces/delays.h>
#include <peripherals/gpio.h>
#include <hwconfig.h>
#include <nvmem_access.h>
#include <spi_bitbang.h>
#include <string.h>
#include <wchar.h>
#include <utils.h>
#include <crc.h>
#include <W25Qx.h>

static const struct W25QxCfg cfg =
{
    .spi = (struct spiDevice*) &flash_spi,
    .cs  = { EFLASH_CS }
};

W25Qx_DEVICE_DEFINE(eflash, cfg, 0x1000000)        // 16 MB, 128 Mbit

const struct nvmPartition memPartitions[] =
{
    {
        .offset = 0x0000,     // First partition Calibration Data
        .size   = 0x2000      // 8KiB
    },
    {
        .offset = 0x2000,     // Second partition Settings
        .size   = 0x2000      // 8KiB
    },
    {
        .offset = 0x4000,     // Third partition Channels
        .size   = 0x18000     // 96KiB
    },
    {
        .offset = 0x1C000,     // Fourth partition Zones
        .size   = 0x40000      // 156KiB
    },
    {
        .offset = 0x5C000,     // Last Partition TODO
        .size   = 0xfa4000     // Remaining space
    },
};

static const struct nvmDescriptor extMem[] =
{
    {
        .name       = "External flash",
        .dev        = &eflash,
        .partNum    = sizeof(memPartitions)/sizeof(struct nvmPartition),
        .partitions = memPartitions
    }
};

void nvm_init()
{
    spiBitbang_init(&flash_spi);
    W25Qx_init(&eflash);
}

void nvm_terminate()
{
    W25Qx_terminate(&eflash);
    spiBitbang_terminate(&flash_spi);
}

const struct nvmDescriptor *nvm_getDesc(const size_t index)
{
    if(index > ARRAY_SIZE(extMem))
        return NULL;

    return &extMem[index];
}

void nvm_readCalibData(void *buf)
{
    (void) buf;
}

void nvm_readHwInfo(hwInfo_t *info)
{
    (void) info;
}

int nvm_readVfoChannelData(channel_t *channel)
{
    (void) channel;

    return -1;
}

int nvm_readSettings(settings_t *settings)
{
    (void) settings;

    return -1;
}

int nvm_writeSettings(const settings_t *settings)
{
    (void) settings;

    return -1;
}

int nvm_writeSettingsAndVfo(const settings_t *settings, const channel_t *vfo)
{
    (void) settings;
    (void) vfo;

    return 0;
}
