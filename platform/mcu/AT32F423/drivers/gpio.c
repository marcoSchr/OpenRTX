//
// Created by marco on 2/24/25.
//

#include <at32f423.h>
#include <peripherals/gpio.h>

enum Speed
{
    LOW    = 0x0,   ///< 2MHz
    MEDIUM = 0x1,   ///< 25MHz
    FAST   = 0x2,   ///< 50MHz
    HIGH   = 0x3    ///< 100MHz
};

void gpio_setMode(void *port, uint8_t pin, enum Mode mode)
{
    gpio_type *p = (gpio_type *)(port);
    p->cfgr  &= ~(3 << (pin*2));
    p->omode &= ~(1 << pin);
    p->pull  &= ~(3 << (pin*2));

    switch(mode)
    {
        case INPUT:
            // (CFGR=00 OMODE=0 PULL=00)
            p->cfgr  |= 0x00 << (pin*2);
            p->omode |= 0x00 << pin;
            p->pull  |= 0x00 << (pin*2);
            break;

        case INPUT_PULL_UP:
            // (MODE=00 TYPE=0 PUP=01)
            p->cfgr  |= 0x00 << (pin*2);
            p->omode |= 0x00 << pin;
            p->pull  |= 0x01 << (pin*2);
            break;

        case INPUT_PULL_DOWN:
            // (MODE=00 TYPE=0 PUP=10)
            p->cfgr  |= 0x00 << (pin*2);
            p->omode |= 0x00 << pin;
            p->pull  |= 0x02 << (pin*2);
            break;

        case ANALOG:
            // (MODE=11 TYPE=0 PUP=00)
            p->cfgr  |= 0x03 << (pin*2);
            p->omode |= 0x00 << pin;
            p->pull  |= 0x00 << (pin*2);
            break;

        case OUTPUT:
            // (MODE=01 TYPE=0 PUP=00)
            p->cfgr  |= 0x01 << (pin*2);
            p->omode |= 0x00 << pin;
            p->pull  |= 0x00 << (pin*2);
            break;

        case OPEN_DRAIN:
            // (MODE=01 TYPE=1 PUP=00)
            p->cfgr  |= 0x01 << (pin*2);
            p->omode |= 0x01 << pin;
            p->pull  |= 0x00 << (pin*2);
            break;

        case ALTERNATE:
            // (MODE=10 TYPE=0 PUP=00)
            p->cfgr  |= 0x02 << (pin*2);
            p->omode |= 0x00 << pin;
            p->pull  |= 0x00 << (pin*2);
            break;

        case ALTERNATE_OD:
            // (MODE=10 TYPE=1 PUP=00)
            p->cfgr  |= 0x02 << (pin*2);
            p->omode |= 0x01 << pin;
            p->pull  |= 0x00 << (pin*2);
            break;

        default:
            // Default to INPUT mode
            p->cfgr  |= 0x00 << (pin*2);
            p->omode |= 0x00 << pin;
            p->pull  |= 0x00 << (pin*2);
            break;
    }
}

void gpio_setAlternateFunction(void *port, uint8_t pin, uint8_t afNum)
{
    gpio_type *p = (gpio_type *)(port);
    afNum &= 0x0F;
    if(pin < 8)
    {
        p->muxl &= ~(0x0F << (pin*4));
        p->muxl |= (afNum << (pin*4));
    }
    else
    {
        pin -= 8;
        p->muxh &= ~(0x0F << (pin*4));
        p->muxh |= (afNum << (pin*4));
    }
}

void gpio_setOutputSpeed(void *port, uint8_t pin, enum Speed spd)
{
    ((gpio_type *)(port))->odrvr &= ~(3 << (pin*2));   // Clear old value
    ((gpio_type *)(port))->odrvr |= spd << (pin*2);    // Set new value
}

void gpio_setPin(void *port, uint8_t pin)
{
    ((gpio_type *)(port))->scr = (1 << pin);
}

void gpio_clearPin(void *port, uint8_t pin)
{
    ((gpio_type *)(port))->scr = (1 << (pin + 16));
}

void gpio_togglePin(void *port, uint8_t pin)
{
    ((gpio_type *)(port))->odt ^= (1 << pin);
}

uint8_t gpio_readPin(const void *port, uint8_t pin)
{
    gpio_type *p = (gpio_type *)(port);
    return ((p->idt & (1 << pin)) != 0) ? 1 : 0;
}