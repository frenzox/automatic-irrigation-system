#include "hal.h"
#include "FreeRTOS.h"
#include "SimpleFSM.h"
#include "timers.h"
#include "task.h"

#include "behavior.h"

#define DRY_SOIL_LIMIT  3500
#define WET_SOIL_LIMIT  2000 

SimpleFSM irrigation_machine;

STATE(IDLE) {
    if(FIRST) {
        turn_pin(PUMP, 0);
        turn_pin(TOP_VALVE, 0);
        turn_pin(BOTTOM_VALVE, 0);
    }

    printf("IDLE\n");
    printf("TOP SENSOR: %d\n", read_analog_input(TOP_SENSOR));
    printf("BOTTOM SENSOR: %d\n", read_analog_input(BOTTOM_SENSOR));

    if(read_analog_input(TOP_SENSOR) > DRY_SOIL_LIMIT &&
            read_analog_input(BOTTOM_SENSOR) > DRY_SOIL_LIMIT)
        NEXT_STATE(IRRIGATING_BOTH);
    else if(read_analog_input(TOP_SENSOR) > DRY_SOIL_LIMIT)
        NEXT_STATE(IRRIGATING_TOP);
    else if(read_analog_input(BOTTOM_SENSOR) > DRY_SOIL_LIMIT)
        NEXT_STATE(IRRIGATING_BOTTOM);
}

STATE(IRRIGATING_BOTH) {
    if(FIRST) {
        turn_pin(TOP_VALVE, 1);
        turn_pin(BOTTOM_VALVE, 1);
        turn_pin(PUMP, 1);
    }

    printf("IRRIGATING BOTH\n");
    printf("TOP SENSOR: %d\n", read_analog_input(TOP_SENSOR));
    printf("BOTTOM SENSOR: %d\n", read_analog_input(BOTTOM_SENSOR));

    if(read_analog_input(TOP_SENSOR) < WET_SOIL_LIMIT &&
            read_analog_input(BOTTOM_SENSOR) < WET_SOIL_LIMIT)
        NEXT_STATE(IDLE);
    else if(read_analog_input(TOP_SENSOR) < WET_SOIL_LIMIT)
        NEXT_STATE(IRRIGATING_BOTTOM);
    else if(read_analog_input(BOTTOM_SENSOR) < WET_SOIL_LIMIT)
        NEXT_STATE(IRRIGATING_TOP);
}

STATE(IRRIGATING_TOP) {
    if(FIRST) {
        turn_pin(TOP_VALVE, 1);
        turn_pin(BOTTOM_VALVE, 0);
        turn_pin(PUMP, 1);
    }

    printf("IRRIGATING TOP\n");
    printf("TOP SENSOR: %d\n", read_analog_input(TOP_SENSOR));
    printf("BOTTOM SENSOR: %d\n", read_analog_input(BOTTOM_SENSOR));

    if(read_analog_input(TOP_SENSOR) > DRY_SOIL_LIMIT &&
            read_analog_input(BOTTOM_SENSOR) > DRY_SOIL_LIMIT)
        NEXT_STATE(IRRIGATING_BOTH);
    else if(read_analog_input(TOP_SENSOR) < WET_SOIL_LIMIT &&
            read_analog_input(BOTTOM_SENSOR) > DRY_SOIL_LIMIT)
        NEXT_STATE(IRRIGATING_BOTTOM);
    else if(read_analog_input(TOP_SENSOR) < WET_SOIL_LIMIT &&
            read_analog_input(BOTTOM_SENSOR) < WET_SOIL_LIMIT)
        NEXT_STATE(IDLE);
}

STATE(IRRIGATING_BOTTOM) {
    if(FIRST) {
        turn_pin(TOP_VALVE, 0);
        turn_pin(BOTTOM_VALVE, 1);
        turn_pin(PUMP, 1);
    }

    printf("IRRIGATING BOTTOM\n");
    printf("TOP SENSOR: %d\n", read_analog_input(TOP_SENSOR));
    printf("BOTTOM SENSOR: %d\n", read_analog_input(BOTTOM_SENSOR));

    if(read_analog_input(TOP_SENSOR) > DRY_SOIL_LIMIT &&
            read_analog_input(BOTTOM_SENSOR) > DRY_SOIL_LIMIT)
        NEXT_STATE(IRRIGATING_BOTH);
    else if(read_analog_input(TOP_SENSOR) > DRY_SOIL_LIMIT &&
            read_analog_input(BOTTOM_SENSOR) < WET_SOIL_LIMIT)
        NEXT_STATE(IRRIGATING_TOP);
    else if(read_analog_input(TOP_SENSOR) < WET_SOIL_LIMIT &&
            read_analog_input(BOTTOM_SENSOR) < WET_SOIL_LIMIT)
        NEXT_STATE(IDLE);
}

void behavior_task(void *pvParameters) {

    INIT(irrigation_machine, IDLE);

    for(;;) {
        EXEC(irrigation_machine);
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}
