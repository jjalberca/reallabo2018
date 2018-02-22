# reallabo2018

## Pinout

-**IN_ 1** está en pin 4
-**IN_2** está en pin 5
-**Enable** en pin 2

| Enable    | IN\_1  | IN\_2  | Sentido      |
| ----------|:------:| ------:|-------------:|
| L         | X      | X      | Stop         |
| H         | L      | L      | Stop         |
| H         | L      | H      | Antihorario  |
| H         | H      | L      | Horario      |
| H         | H      | H      | Stop         |


Cables amarillo y blanco del motor son para el encoder (interrupciones)

Relación entre pines del micro y de la placa:
- [Arduino oficial PinMapping SAM3X](https://www.arduino.cc/en/Hacking/PinMappingSAM3X)
- [Arduinooficial Schematic](https://www.arduino.cc/en/uploads/Main/arduino-Due-schematic.pdf)
- [*The Unofficial Arduino Due Pinout Diagram*](http://www.robgray.com/temp/Due-pinout.svg)

## DOC
[Atmel SAM3X8E Datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf)

PWM en punto 38

[Documentacion ASF](http://asf.atmel.com/docs/latest/)

## Build
### Prerequisites

Necesitamos el compilador `arm-none-eabi-gcc`, podemos descargar los binarios desde la página oficial de [ARM Developers](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads), los podemos descomprimir en `/opt/gcc-none-eabi-gcc`.

```sh
tar -xjvf gcc-arm-none-eabi-*-linux.tar.bz2
sudo cp gcc-arm-none-eabi-*-major /opt/gcc-arm-none-eabi
rm -rf gcc-arm-none-eabi*
```

Necesitamos *BOSSA* para poder subir los binarios compilados al microprocesador. Lo podemos instalar desde la fuente en su [GitHub](https://github.com/shumatech/BOSSA) o en ubuntu con `apt install bossa-cli`.

En la carpeta hardware están las librerías de Atmel para el microprocesador, sacadas de  [página oficial][ATMEL ASF] y del paquete del IDE de Arduino para DUE.

**Es posible que falte alguna dependencia**

[ATMEL ASF]: https://www.microchip.com/avr-support/advanced-software-framework-(asf)

### Makefile
El makefile tiene todo lo necesario para compilar binarios de *C* y *C++* y subirlos a la placa mediante bossa.

Los ficheros fuentes deben estar en `src` y las cabeceras en `include`. El archivo `due_sam3x.init.c` contiene la rutina de inicialización que puede ser modificada para cambiar el comportamiento.

Para compilar:
```sh
make
make install
```

A veces es necesario pulsar el botón *erase* de la placa antes de ejecutar `make install` para que funcione correctamente. Se puede cambiar el puerto serie al que está conectado la placa con `make install UPLOAD_PORT=ttyACM0`.
