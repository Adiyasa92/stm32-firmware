#include <stm32f4xx.h>
#include "switch.h"
#include "ecu.h"

extern ecu_t ecu;

void switch_init(void)
{
    SWITCH_GPIO->MODER &= ~0x0000FFFF;
    SWITCH_GPIO->PUPDR |= 0x00005555;
}

void switch_update(void)
{
    uint32_t flags, sw;

    sw = SWITCH_GPIO->IDR;
    flags = 0;

    if ((sw & SWITCH_IGN_ODR))
    {
        flags |= STATUS_FLAGS2_IGN_SW;
    }
    else if ((sw & SWITCH_START_ODR))
    {
        flags |= STATUS_FLAGS2_START_SW;
    }

    __disable_irq();
    ecu.status.flags2 &= ~(STATUS_FLAGS2_IGN_SW | STATUS_FLAGS2_START_SW);
    ecu.status.flags2 |= flags;
    __enable_irq();
}
