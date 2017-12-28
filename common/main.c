#include <stm32f1xx.h>

#include "hal.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"

#define LED_PIN 5
#define LED_ON() GPIOA->ODR |= (1 << LED_PIN)
#define LED_OFF() GPIOA->ODR &= (~(1 << LED_PIN))

void blink_task(void *pvParameters) {
    while(1) {
        LED_ON();
        vTaskDelay(1000/portTICK_PERIOD_MS);
        LED_OFF();
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

int main() {

    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL = (1 << 20); // SET OUTPUT MODE

    LED_ON();
    BaseType_t result = xTaskCreate(blink_task, "blink_task", 128, NULL, 1, NULL);
    LED_OFF();
    if(result != pdPASS)
        LED_ON();
    vTaskStartScheduler();
    while(1){}
    return 0;
}


