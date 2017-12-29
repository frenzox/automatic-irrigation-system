#include <stdarg.h>

#include "hal.h"
#include "SEGGER_RTT.h"


extern int SEGGER_RTT_vprintf(unsigned BufferIndex, const char * sFormat, va_list * pParamList);

void printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    SEGGER_RTT_vprintf(0, format, &args);
    va_end(args);
}

void turn_pin(uint32_t pin, uint32_t status) {
    if(status) {
        GPIOA->ODR |= (1 << pin);
    } else {
        GPIOA->ODR &= (~(1 << pin));
    }
}

uint32_t read_pin(uint32_t pin) {
    return ((GPIOA->ODR & (1 << pin)) >> pin);
}

void toggle_pin(uint32_t pin) {
    turn_pin(pin, (~read_pin(pin) & 1));
}

void adc_init() {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    ADC1->CR2 = 0x1;
    GPIOA->CRL &= ~1; // SET PA0 AS ANALOG INPUT
    ADC1->CR2 |= (1 << 2);
    while(!((ADC1->CR2 >> 2) & 1)){}
}

int32_t read_top_moisture_sensor() {
    ADC1->CR2 |= 1;
    while(!((ADC1->SR >> 1) & 1)){}
    return ADC1->DR & 0xFFF;
}

int32_t read_bottom_moisture_sensor() {
    return 0;
}

void init_hal() {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL = (1 << 20); // SET OUTPUT MODE
}
