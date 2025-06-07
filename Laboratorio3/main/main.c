#include <stdio.h>
#include "esp_log.h"
#include "led_embebido.h"
#include "esp_rom_delay_ms.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

#define UART_PORT UART_NUM_0
#define UART_TXD  (GPIO_NUM_1)
#define UART_RXD  (GPIO_NUM_3)
#define UART_BAUD_RATE 115200
#define UART_BUF_SIZE 1024


// nueva Task A. Esta tarea se encarga de llamar a la task C y 
// agregarle un delay a la tarea antes de mandarla a la queue.
void vTaskA( void *pvParameters ) {
    while(1) {
            for(;;);
        }
    vTaskDelete(NULL);
}

// nueva Task C. Esta tarea se encarga de de encender y apagar un LED, 
// y ponerlo con un color que lee de una queue.
void vTaskC( void *pvParameters ) {
    while(1) {
            for(;;);
        }
    vTaskDelete(NULL);
}

// nueva Task B.
// Esta tarea se encargará de gestionar el USB/UART y procesar los comandos que le llegan.
// Si el parámetro de tiempo es cero, TASK B colocará directamente el comando en el
// queue. Si el parámetro de tiempo es distinto de cero, TASK B creará un timer, y será el
// callback del timer que pondrá el comando en el queue.
void vTaskB( void *pvParameters ) {
    while(1) {
            for(;;);
        }
    vTaskDelete(NULL);
}

void app_main(void)
{
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    // Driver UART sin eventos
    ESP_ERROR_CHECK(uart_driver_install(UART_PORT, UART_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_PORT, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_PORT, UART_TXD, UART_RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    
    //uart_read_bytes() y uart_write_bytes()
}
