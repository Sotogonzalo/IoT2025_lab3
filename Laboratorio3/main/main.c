#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "led_embebido.h"
#include "taskA.h"
#include "taskB.h"
#include "taskC.h"
#include "color_shared.h"

led_color_t color_global = {255, 0, 0, 1};
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

    // Cola de comandos (tiene elementos led_color_t)
    xQueueComandos = xQueueCreate(10, sizeof(led_color_t));
    if (xQueueComandos == NULL) {
        printf("Error al crear la queue\n");
        return;
    }

    // Inicio de tareas
    start_task_a(semaforo_color, &color_global); // LED parpadea con color protegido
    start_task_b(xQueueComandos); // Recibe comandos por UART y los encola
    start_task_c(xQueueComandos, semaforo_color, &color_global);  // Toma comandos de la cola y actualiza el color compartido
}
