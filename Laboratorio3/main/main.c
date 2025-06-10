#include <stdio.h>

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
    
}
