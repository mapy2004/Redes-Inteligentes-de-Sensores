Comandos principales para cargar cualquier programa:
Primero abrir el docker mediante:
contiker
luego acceder a la carpeta donde se encuenta el .c en cuestión, posteriormente tienes que transformar el .c a formato hexadecimal para que lo pueda leer el micro, mediante:
make TARGET=nrf52840 BOARD=dongle
después generamos un .zip del hex que meteremos posteriormente en la placa:
nrfutil pkg generate --hw-version 52 --sd-req 0x00 --debug-mode --application build/nrf52840/dongle/ejercicio4.hex dfu.zip
(En este caso esta puesta la ruta del ejercicio4 en cuestión, cambiar la ruta en base al archivo a enviar)
Una vez creado el zip, tendríamos que enviarlo a la placa:
nrfutil dfu usb-serial -pkg dfu.zip -p /dev/ttyACM0 (donde ttyACM0 es la placa en la posición 0 del puerto usb)
por útlimo tienes que meterte a la propia placa para "escuchar" lo que envía por consola:
make TARGET=nrf52840 BOARD=dongle PORT=/dev/ttyACM0 login.


Hola prueba
