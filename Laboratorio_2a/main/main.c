#include <stdio.h>
#include "esp_log.h"
#include "led_embebido.h"
#include "touch_embebido.h"
#include "esp_rom_delay_ms.h"
#include "esp_task_wdt.h"

static const char *TAG = "main";

typedef enum {
    CANAL_COLOR = 0,  // TOUCH2 (photo) - prender/apagar LED
    CANAL_ONOFF,      // TOUCH6 (play/pause) - cambiar color
    CANAL_BRILLO,     // TOUCH11 (network) - cambiar brillo
    NUM_CANALES
} canal_touch_t;

// Colores RGB
static const uint8_t colores[][3] = {
    {255, 0, 0}, {0, 255, 0}, {0, 0, 255},
    {255, 255, 0}, {255, 0, 255}, {0, 255, 255},
    {255, 255, 255}
};
#define NUM_COLORES (sizeof(colores)/sizeof(colores[0]))

void app_main(void) {
    esp_task_wdt_deinit();
    ESP_LOGI(TAG, "Iniciando aplicación...");

    if (led_embebido_iniciar() != ESP_OK) {
        ESP_LOGE(TAG, "No se pudo inicializar el LED");
        return;
    }

    if (touch_embebido_iniciar() != ESP_OK) {
        ESP_LOGE(TAG, "No se pudo inicializar touch");
        return;
    }

    bool prev_estado[NUM_CANALES] = {false};
    int color_actual_idx = 0;
    uint8_t brillo_actual = 255;
    bool led_encendido = true;

    // LED inicial
    led_embebido_set_brillo(brillo_actual);
    led_embebido_set_color(
        colores[color_actual_idx][0],
        colores[color_actual_idx][1],
        colores[color_actual_idx][2]
    );

    while (1) {
        for (int i = 0; i < NUM_CANALES; i++) {
            bool tocado = touch_embebido_fue_tocado(i);

            if (tocado && !prev_estado[i]) {
                ESP_LOGI(TAG, "Canal %d activado", i);

                switch (i) {
                    case CANAL_COLOR:
                        color_actual_idx = (color_actual_idx + 1) % NUM_COLORES;
                        ESP_LOGI(TAG, "Color: #%d (R:%d G:%d B:%d)", 
                            color_actual_idx,
                            colores[color_actual_idx][0],
                            colores[color_actual_idx][1],
                            colores[color_actual_idx][2]);
                        if (led_encendido) {
                            led_embebido_set_color(
                                colores[color_actual_idx][0],
                                colores[color_actual_idx][1],
                                colores[color_actual_idx][2]);
                        }
                        break;

                    case CANAL_ONOFF:
                        led_encendido = !led_encendido;
                        ESP_LOGI(TAG, "LED %s", led_encendido ? "ENCENDIDO" : "APAGADO");
                        if (led_encendido) {
                            led_embebido_set_color(
                                colores[color_actual_idx][0],
                                colores[color_actual_idx][1],
                                colores[color_actual_idx][2]);
                        } else {
                            led_embebido_apagar();
                        }
                        break;

                    case CANAL_BRILLO:
                        brillo_actual += 64;
                        if (brillo_actual > 255) brillo_actual = 0;
                        ESP_LOGI(TAG, "Brillo: %d", brillo_actual);
                        led_embebido_set_brillo(brillo_actual);
                        if (led_encendido && brillo_actual > 0) {
                            led_embebido_set_color(
                                colores[color_actual_idx][0],
                                colores[color_actual_idx][1],
                                colores[color_actual_idx][2]);
                        } else if (brillo_actual == 0) {
                            led_embebido_apagar();
                        }
                        break;
                }
            }

            prev_estado[i] = tocado;
        }
        esp_rom_delay_ms(100);
    }
}
