# reallabo2018

## Pinout

- **IN_ 1** está en pin 4
- **IN_2** está en pin 5
- **Enable** en pin 2

| Enable    | IN\_1  | IN\_2  | Sentido      |
| ----------|:------:| ------:|-------------:|
| L         | X      | X      | Stop         |
| H         | L      | L      | Stop         |
| H         | L      | H      | Antihorario  |
| H         | H      | L      | Horario      |
| H         | H      | H      | Stop         |


Cables amarillo y blanco del motor son para el encoder (interrupciones).
Y el Azul y el Verde son 3.3V y GND en ese orden.

Relación entre pines del micro y de la placa:
- [Arduino oficial PinMapping SAM3X](https://www.arduino.cc/en/Hacking/PinMappingSAM3X)
- [Arduino oficial Schematic](https://www.arduino.cc/en/uploads/Main/arduino-Due-schematic.pdf)
- [*The Unofficial Arduino Due Pinout Diagram*](http://www.robgray.com/temp/Due-pinout.svg)

## DOC
[Atmel SAM3X8E Datasheet][SAM3X/A Datasheet]

[Documentacion ASF](http://asf.atmel.com/docs/latest/)

[Documentacion ASF - PWM](http://asf.atmel.com/docs/latest/sam3x/html/sam_pwm_quickstart.html)

Definiciones de constantes y flags están en `sdk-asf-3.37/sam/utils/cmsis/sam3x/include/component`.

[SAM3X/A Datasheet]: http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf

## Build
### Prerequisites

Necesitamos el compilador `arm-none-eabi-gcc`, podemos descargar los binarios desde la página oficial de [ARM Developers](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads), los podemos descomprimir en `/opt/gcc-none-eabi-gcc`.

```sh
tar -xjvf gcc-arm-none-eabi-*-linux.tar.bz2
sudo cp gcc-arm-none-eabi-*-major /opt/gcc-arm-none-eabi
rm -rf gcc-arm-none-eabi*
```

Necesitamos *BOSSA* para poder subir los binarios compilados al microprocesador. Lo podemos instalar desde la fuente en su [GitHub](https://github.com/shumatech/BOSSA) o en Ubuntu con `apt install bossa-cli`.

Necesitamos el framework de ATMEL que debemos descargar desde [ATMEL ASF] y descomprimirlo en algún lugar del ordenador que debe ser indicado al Makefile mediante una variable de entorno (`make ASF_PATH=/path/to/asf/root`)o modificando su configuración (Actualmente está configurado para que la carpeta de ASF esté dentro del repositorio).

**Es posible que falte alguna dependencia**

[ATMEL ASF]: https://www.microchip.com/avr-support/advanced-software-framework-(asf)

### Makefile
El makefile tiene todo lo necesario para compilar binarios de *C* y *C++* y subirlos a la placa mediante bossa.

Los ficheros fuentes deben estar en `src` y las cabeceras en `include`. Son necesarios unos archivos de inicialización de la placa que deben estár en `include`.

Para añadir drivers y funciones del framework es necesario modificar el archivo `config.mk` y añadir tanto las cabeceras como las fuentes a las variables `ASF_INC` y `ASF_SRC`.

Para compilar:
```sh
make
make flash
```

A veces es necesario pulsar el botón *erase* de la placa antes de ejecutar `make flash` para que funcione correctamente. Se puede cambiar el puerto serie al que está conectado la placa con `make install UPLOAD_PORT=ttyACM0`.

## Timers Counters.
Tanto para las interrupciones periódicas que necesitemos como para generar el PWM del motor se usarán los TC del SAM3X8E.
Tenemos 3 *timers* con 3 canales cada uno y cada uno de los canales con dos salidas PWM. También existe un periférico de PWM pero ninguna de las salidas con las que cuenta coincide con los pines que necesitamos.

### Aclaración de la nomenclatura de NVIC.
Para las funciones del **Driver TC** existen únicamente tres *timers* (TC0, TC1 y TC2) y se elige el canal al que nos estamos refiriendo mediante un parametro de las funciones. Sin embargo para el **PCM** y el **NVIC** existen 9 *timers* independientes (TC[0-8]), uno por cada canal.

Estas dos nomenclaturas se pueden ver en el apartado 7.1 (pág. 31) del [SAM3X/A Datasheet]

### Quadrature decoder
El SAM3XE8 contiene un decodificador de las señales del encoder del motor en hardware, que permite controlar su posición y/o su velocidad sin ningún software adicional, usando los canales 0, 1 y 2 del TC0 para este propósito. Sin embargo, no es facil de integrar en este proyecto, ya que necesariamente las entradas a este decodificador deben ser TIOA0 y TIOB0.
TIOA0 coincide con la entrada de *Enable_1* del puente H, por lo que habría que modificar la *"shield"* del puente H.

La documentación está en el apartado 36.6.14 (pág. 871) del [SAM3X/A Datasheet]
