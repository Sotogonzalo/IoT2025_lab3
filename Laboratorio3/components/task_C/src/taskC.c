#include "taskC.h"
#include "led_embebido.h"
#include "esp_log.h"
#include <stdlib.h>
#include <inttypes.h>

static const char *TAG = "TaskC";

static QueueHandle_t xQueueComandos = NULL;
static SemaphoreHandle_t xSemaforoColor = NULL;
static uint8_t *ptr_r, *ptr_g, *ptr_b;

// actualiza el color protegido por semáforo
static void timer_callback(TimerHandle_t xTimer) {
    comando_color_t *comando = (comando_color_t *) pvTimerGetTimerID(xTimer);

    if (xSemaphoreTake(xSemaforoColor, portMAX_DELAY)) {
        *ptr_r = comando->r;
        *ptr_g = comando->g;
        *ptr_b = comando->b;
        xSemaphoreGive(xSemaforoColor);

        ESP_LOGI(TAG, "Color actualizado por timer: R=%d G=%d B=%d", comando->r, comando->g, comando->b);
    }

    free(comando);
    xTimerDelete(xTimer, 0);
}

static void vTaskC(void *pvParameters) {
    while (1) {
        comando_color_t *comando = malloc(sizeof(comando_color_t));
        if (!comando) {
            ESP_LOGE(TAG, "Fallo malloc comando");
            continue;
        }

        if (xQueueReceive(xQueueComandos, comando, portMAX_DELAY) == pdTRUE) {
            TimerHandle_t xTimer = xTimerCreate("CmdTimer",
                                                pdMS_TO_TICKS(comando->delay_segundos * 1000),
                                                pdFALSE, // one-shot
                                                comando,
                                                timer_callback);

            if (xTimer != NULL) {
                if (xTimerStart(xTimer, 0) != pdPASS) {
                    ESP_LOGE(TAG, "Fallo al iniciar el timer");
                    free(comando);
                    xTimerDelete(xTimer, 0);
                } else {
                    ESP_LOGI(TAG, "Timer iniciado por %" PRIu32 " s", comando->delay_segundos);
                }
            } else {
                ESP_LOGE(TAG, "Fallo al crear el timer");
                free(comando);
            }
        } else {
            free(comando);
        }
    }

    vTaskDelete(NULL);
}

void start_task_c(QueueHandle_t queue, SemaphoreHandle_t semaforo, uint8_t *color_r, uint8_t *color_g, uint8_t *color_b) {
    xQueueComandos = queue;
    xSemaforoColor = semaforo;
    ptr_r = color_r;
    ptr_g = color_g;
    ptr_b = color_b;

    xTaskCreate(vTaskC, "task_c", 4096, NULL, 5, NULL);
}
