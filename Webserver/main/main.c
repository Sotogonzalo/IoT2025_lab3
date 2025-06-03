#include <stdio.h>
#include "wifi_embebido.h"
#include "servidor_embebido.h"

void app_main(void)
{
    printf("Iniciando conexión WiFi...\n");

    // Ejemplo para conectarse a una red WiFi, STA.
    iniciar_wifi_sta("Nombre_de_red", "Contraseña_secreta");

    // Ejemplo para crear una red WiFi AP.
    // iniciar_wifi_ap("Wifi_prueba", "top_secret_password");

    // Iniciar el servidor web
    // http://192.168.1.20
    iniciar_servidor_web();
}
