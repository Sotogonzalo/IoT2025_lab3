#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "led_embebido.h"
#include "taskA.h"

led_color_t color_global = {255, 0, 0};
SemaphoreHandle_t semaforo_color;

void app_main(void) {
    led_embebido_iniciar();
    semaforo_color = xSemaphoreCreateMutex();

    if (semaforo_color == NULL) {
        printf("Error al crear el mutex\n");
        return;
    }

    start_task_a(semaforo_color, &color_global);
}
