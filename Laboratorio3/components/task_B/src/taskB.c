#include "taskB.h"
#include "driver/uart.h"
#include "esp_log.h"
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#define UART_NUM UART_NUM_1
#define BUF_SIZE 128
#define TAG "TASK_B"

// #define ECHO_TEST_TXD (CONFIG_EXAMPLE_UART_TXD)
// #define ECHO_TEST_RXD (CONFIG_EXAMPLE_UART_RXD)
// #define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
// #define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

// #define ECHO_UART_PORT_NUM (CONFIG_EXAMPLE_UART_PORT_NUM)
// #define ECHO_UART_BAUD_RATE (CONFIG_EXAMPLE_UART_BAUD_RATE)
// #define ECHO_TASK_STACK_SIZE (CONFIG_EXAMPLE_TASK_STACK_SIZE)

static QueueHandle_t taskB_queue = NULL;

//imprime la task en consola (propósito de depuración)
static void print_queue(QueueHandle_t queue) {
    if (queue == NULL) {
        ESP_LOGE(TAG, "Queue is not initialized");
        return;
    }

    led_command_t cmd;
    while (xQueueReceive(queue, &cmd, 0) == pdTRUE) {
        ESP_LOGI(TAG, "Comando: R:%d G:%d B:%d Delay:%" PRIu32, cmd.red, cmd.green, cmd.blue, cmd.delay_seconds);
    }
}

static void parse_command(const char *input, led_command_t *cmd) {
    char color[16];
    int delay;

    if (sscanf(input, "%15s %d", color, &delay) == 2) {
        cmd->delay_seconds = delay;

        if (strcasecmp(color, "rojo") == 0) {
            cmd->red = 255; cmd->green = 0; cmd->blue = 0;
        } else if (strcasecmp(color, "verde") == 0) {
            cmd->red = 0; cmd->green = 255; cmd->blue = 0;
        } else if (strcasecmp(color, "azul") == 0) {
            cmd->red = 0; cmd->green = 0; cmd->blue = 255;
        } else {
            cmd->red = cmd->green = cmd->blue = 0;
        }
    }
}

static void vTaskB(void *pvParameters) {
    uint8_t *data = malloc(BUF_SIZE);
    while (1) {
        int len = uart_read_bytes(UART_NUM, data, BUF_SIZE - 1, pdMS_TO_TICKS(100));
        if (len > 0) {
            data[len] = '\0';
            ESP_LOGI(TAG, "Recibido: %s", (char *)data);
            led_command_t cmd;
            parse_command((char *)data, &cmd);

            // Enviamos a la cola
            if (xQueueSend(taskB_queue, &cmd, portMAX_DELAY) != pdPASS) {
                ESP_LOGE(TAG, "No se pudo enviar el comando a la cola");
            }
        }

        // Imprimir la cola para depuración
        print_queue(taskB_queue);
    }
    free(data);
    vTaskDelete(NULL);
}

void taskB_uart_init(void) {
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, 17, 18, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // uart_config_t uart_config = {
    //     .baud_rate = 115200,
    //     .data_bits = UART_DATA_8_BITS,
    //     .parity = UART_PARITY_DISABLE,
    //     .stop_bits = UART_STOP_BITS_1,
    //     .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    //     .source_clk = UART_SCLK_APB,
    // };

    // ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    // ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    // ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));
}

void start_task_b(QueueHandle_t queue) {
    taskB_queue = queue;
    xTaskCreate(vTaskB, "TaskB", 4096, NULL, 10, NULL);
}
