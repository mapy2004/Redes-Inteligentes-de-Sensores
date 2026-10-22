# Guía de Desarrollo, Flasheo y Monitorización
**Contiki-NG en nRF52840 Dongle (Entorno Docker / VirtualBox)**
Esta guía detalla el procedimiento paso a paso para compilar aplicaciones de Contiki-NG, flashearlas en la placa nRF52840 Dongle y monitorizar su salida serie.
## 1. Preparación previa (Dispositivo y USB)
* **Qué se hace:** Se prepara el hardware y se asegura la comunicación por USB entre el sistema físico, VirtualBox y la máquina virtual.
* **Pasos:**
  1. Conecta la placa nRF52840 Dongle al puerto USB del ordenador.
  2. Asegúrate de que VirtualBox ha capturado el dispositivo (si no se conecta automáticamente, ve a *Dispositivos > USB* en la barra de VirtualBox y selecciónalo).
  3. Pon la placa en **modo DFU**: Pulsa una vez el botón físico de reset de la placa para que el LED rojo comience a parpadear.
  4. Comprueba en la terminal anfitriona que el puerto está listo ejecutando:
     
     ls -l /dev/ttyACM*
    
     *(Debe devolver la ruta `/dev/ttyACM0`).*

## 2. Compilar y Flashear (Dentro de Docker)
* **Qué se hace:** Se utiliza el contenedor de Docker (`contiker`), que contiene todo el toolchain y las herramientas de compilación necesarias para generar el binario y enviarlo a la placa.
* **Pasos:**
  1. Entra al contenedor de Docker de Contiki-NG:
     
     contiker
    
  2. Navega hasta la carpeta de tu práctica o proyecto:
     
     cd ~/contiki-ng/Redes-Inteligenetes-de-Sensores/
     
  3. Lanza el comando de compilación y subida automática 
     
     make TARGET=nrf52840 BOARD=dongle nombre-de-tu-app.dfu-upload
     
  4. Espera a que el proceso finalice y aparezca el mensaje de confirmación **`Device programmed`** al 100%.


## 3. Visualizar los mensajes (Fuera de Docker)
* **Qué se hace:** Se sale del entorno de compilación para usar las herramientas de monitorización del sistema anfitrión, abriendo el puerto serie para leer la salida de texto de la aplicación.
* **Pasos:**
  1. Sal del contenedor Docker escribiendo:
   
     exit
     
  2. Asegúrate de que el puerto USB vuelve a estar disponible tras el reinicio de la placa (`ls -l /dev/ttyACM*`).
  3. Abre el monitor serie (`picocom`) con la configuración de baudios adecuada:
     
     picocom -fh -b 115200 --imap lfcrlf /dev/ttyACM0
    
  4. **Pulsa una vez el botón de reset físico** de la placa para reiniciar el programa y visualizar el flujo de mensajes en pantalla.

> **Nota útil:** Para cerrar el monitor `picocom` cuando termines, pulsa las teclas `Ctrl + A` y a continuación `Ctrl + X`.