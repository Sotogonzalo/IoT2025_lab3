#ifndef TASK_C_H
#define TASK_C_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

typedef struct {
    uint8_t r, g, b;
    uint32_t delay_segundos;
} comando_color_t;

// Crea la tarea C con la queue y el semáforo compartido
void start_task_c(QueueHandle_t queue, SemaphoreHandle_t semaforo, uint8_t *color_r, uint8_t *color_g, uint8_t *color_b);

#endif // TASK_C_H
