#ifndef TASK_A_H
#define TASK_A_H

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

// Estructura que contiene el color RGB
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} led_color_t;

// Función para crear la tarea A
void start_task_a(SemaphoreHandle_t mutex_color, led_color_t *shared_color);

#endif // TASK_A_H
