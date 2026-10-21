## Comandos principales para cargar cualquier programa:
1. Abrir el docker mediante:

contiker

2. Acceder a la carpeta donde se encuenta el .c en cuestión, posteriormente tienes que transformar el .c a formato hexadecimal para que lo pueda leer el micro, mediante:

make TARGET=nrf52840 BOARD=dongle

2. Generamos un .zip del hex que meteremos posteriormente en la placa (En este caso esta puesta la ruta del ejercicio4 en cuestión, cambiar la ruta en base al archivo a enviar):

nrfutil pkg generate --hw-version 52 --sd-req 0x00 --debug-mode --application build/nrf52840/dongle/ejercicio4.hex dfu.zip

3. Una vez creado el zip, tendríamos que enviarlo a la placa (donde ttyACM0 es la placa en la posición 0 del puerto usb):

nrfutil dfu usb-serial -pkg dfu.zip -p /dev/ttyACM0 

4. Por útlimo tienes que meterte a la propia placa para "escuchar" lo que envía por consola:

make TARGET=nrf52840 BOARD=dongle PORT=/dev/ttyACM0 login.



## Comando para simularlo en nativo 
1. En la carpeta del proyecto donde está el .c ejecutar el comando

make TARGET=native

2. Buscar el programa compilado y ejecutarlo

cd build/native && ./ejercicio.native


