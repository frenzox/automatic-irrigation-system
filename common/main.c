#include "hal.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "task.h"
#include "behavior.h"

int main() {

    init_hal();
    adc_init();

    xTaskCreate(behavior_task, "behavior_task", 128, NULL, 1, NULL);

    vTaskStartScheduler();
    return 0;
}


