set(component_srcs "main.c")

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS "."
                       PRIV_INCLUDE_DIRS ""
                       PRIV_REQUIRES "driver" "esp_rom_delay_ms" "led_embebido" "touch_embebido"
                       REQUIRES "")
