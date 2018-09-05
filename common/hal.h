#ifndef HAL_H
#define HAL_H

#define PUMP            5
#define TOP_SENSOR      0
#define BOTTOM_SENSOR   1
#define TOP_VALVE       6
#define BOTTOM_VALVE    7

void init_hal();
void printf(const char *format, ...);
void turn_pin(uint32_t pin, uint32_t status);
void toggle_pin(uint32_t pin);
uint32_t read_pin(uint32_t pin);
int32_t read_analog_input(uint32_t pin);
void adc_init();
int32_t read_top_moisture_sensor();

#endif //HAL_H
