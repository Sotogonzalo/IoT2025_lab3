#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "led_embebido.h"
#include "taskA.h"
#include "taskB.h"
#include "taskC.h"

led_color_t color_global = {255, 0, 0};
SemaphoreHandle_t semaforo_color;
QueueHandle_t xQueueComandos;

void app_main(void) {
    // Inicializo LED
    led_embebido_iniciar();

    // Inicializo UART
    taskB_uart_init();

    // Semáforo
    semaforo_color = xSemaphoreCreateMutex();
    if (semaforo_color == NULL) {
        printf("Error al crear el mutex\n");
        return;
    }

    // Cola de comandos
    xQueueComandos = xQueueCreate(10, sizeof(comando_color_t));
    if (xQueueComandos == NULL) {
        printf("Error al crear la queue\n");
        return;
    }

    // Iniciar tareas
    start_task_a(semaforo_color, &color_global);
    start_task_b(xQueueComandos);
    // start_task_c(xQueueComandos, semaforo_color, &color_global.r, &color_global.g, &color_global.b);
}
