#include "hal.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"

void blink_task(void *pvParameters) {
    int32_t sensor_value;
    while(1) {
        toggle_pin(5);
        sensor_value = read_analog_input(0);
        printf("SENSOR 1 VALUE: %d\n", sensor_value);
        sensor_value = read_analog_input(1);
        printf("SENSOR 2 VALUE: %d\n", sensor_value);
        vTaskDelay(500/portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

int main() {

    init_hal();
    adc_init();

    xTaskCreate(blink_task, "blink_task", 128, NULL, 1, NULL);

    vTaskStartScheduler();
    return 0;
}


