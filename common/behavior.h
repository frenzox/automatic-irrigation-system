#ifndef BEHAVIOR_H
#define BEHAVIOR_H
#include "SimpleFSM.h"

STATE(IDLE);
STATE(IRRIGATING_BOTH);
STATE(IRRIGATING_TOP);
STATE(IRRIGATING_BOTTOM);

void behavior_task(void *pvParameters);

#endif //BEHAVIOR_H
