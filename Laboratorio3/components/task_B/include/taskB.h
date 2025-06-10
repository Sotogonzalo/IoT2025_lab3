#ifndef TASK_B_H
#define TASK_B_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// Estructura para los comandos leídos desde UART
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint32_t delay_seconds;
} led_command_t;

// Inicialización del UART
void taskB_uart_init(void);

// Inicialización de la TaskB
void start_task_b(QueueHandle_t queue);

#endif // TASK_B_H
