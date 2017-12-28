#include "hal.h"
#include "stm32f1xx.h"

static __IO uint32_t us_ticks;

static void delay_init() {
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock/1E6);
}

void delay_us(uint32_t us) {
    us_ticks = us;
    while(us_ticks);
}

void delay_ms(uint32_t ms) {
    while(ms--)
        delay_us(1E3);
}

void SysTick_Handler() {
    if(us_ticks != 0) 
        us_ticks--;
}

void init_hal() {
    delay_init();
}
