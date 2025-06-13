En este laboratorio se implementaron 4 librerias color_shared, task_A, task_B y task_C, las cuales se encargan de todas las funcionalidades del laboratorio.
Para compilar el proyecto le damos a Build, flash and monitor. Una vez en ejecución podemos escribir en el Monitor Device de ESP-IDF los comandos.
Estos comandos son ("color delay"): "rojo 2", "verde 3", "azul 1", "rojo 1"...
Estos comando serán recibidos por la task B que procesará el input y lo pondrá en una cola.
La task C se encargará de recibir los mensajes de la cola y modificar una variable que contiene el delay y color del led.
Y la task A se encargará de parpadear el led con el color que lee de la variable previamente setteada por la task C, y con el delay correspondiente.