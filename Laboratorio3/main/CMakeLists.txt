set(component_srcs "main.c")

idf_component_register(SRCS "${component_srcs}"
                       INCLUDE_DIRS "."
                       PRIV_INCLUDE_DIRS ""
                       PRIV_REQUIRES "driver" "esp_rom_delay_ms" "led_embebido" "task_A" "task_B" "task_C" "color_shared"
                       REQUIRES "")
