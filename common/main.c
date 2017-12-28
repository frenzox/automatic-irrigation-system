#include <stm32f1xx.h>
#include "hal.h"

#define LED_PIN 5
#define LED_ON() GPIOA->ODR |= (1 << LED_PIN)
#define LED_OFF() GPIOA->ODR &= (~(1 << LED_PIN))

int main() {

    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL = (1 << 20); // SET OUTPUT MODE

    init_hal();
    
    while(1){
        LED_ON();
        delay_ms(1000);
        LED_OFF();
        delay_ms(1000);
    }
    return 0;
}

