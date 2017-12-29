#ifndef HAL_H
#define HAL_H
#include "stm32f1xx.h"

void init_hal();
void printf(const char *format, ...);
void turn_pin(uint32_t pin, uint32_t status);
void toggle_pin(uint32_t pin);
uint32_t read_pin(uint32_t pin);
int32_t read_top_moisture_sensor();
void adc_init();

#endif //HAL_H
