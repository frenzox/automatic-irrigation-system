#include <stdarg.h>

#include "hal.h"
#include "SEGGER_RTT.h"

#define NUMBER_OF_SENSORS   2

static volatile int32_t sensors[NUMBER_OF_SENSORS];

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
    GPIOA->CRL &= ~(1 << 1); // SET PA1 AS ANALOG INPUT
    ADC1->CR1 |= (1 << 8); // SET SCAN MODE
    ADC1->CR2 |= (1 << 1); // SET CONTINUOUS MODE
    ADC1->CR2 |= (1 << 8); // ENABLE ADC TO DMA

    ADC1->SQR1 |= (1 << 20);
    ADC1->SQR3 = 0;
    ADC1->SQR3 |= (1 << 5);

    ADC1->CR2 |= (1 << 2); // START CALIBRATION
    while(!((ADC1->CR2 >> 2) & 1)){}
    ADC1->CR2 |= 0x1;
}

void dma_init() {
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;
    DMA1_Channel1->CMAR = (uint32_t)sensors;
    DMA1_Channel1->CNDTR = NUMBER_OF_SENSORS;
    DMA1_Channel1->CCR |= (1 << 9);  //PSIZE = 16-bits
    DMA1_Channel1->CCR |= (1 << 11); //MSIZE = 16-bits
    DMA1_Channel1->CCR |= (1 << 7);  //Memory auto-increment
    DMA1_Channel1->CCR |= (1 << 5); //CIRC, circular mode
    DMA1_Channel1->CCR |= 1; //EN
}

void gpio_init() {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL = (1 << 20); // SET OUTPUT MODE
}

int32_t read_analog_input(uint32_t pin) {
    return sensors[pin];
}

void init_hal() {
    gpio_init();
    dma_init();
    adc_init();
}
