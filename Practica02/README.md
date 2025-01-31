# Práctica - Búsquedas Informadas

Esta práctica consiste en un programa que resuelve laberintos y genera resultados tanto en formato visual como en tablas, facilitando su comprensión. A continuación, se detallan los pasos necesarios para compilar y ejecutar el programa, así como las recomendaciones para visualizar correctamente los resultados.

---

## **COMPILAR**

Para compilar el programa, siga estos pasos:

1. Abra una terminal y navegue al directorio `build` del proyecto.
2. Ejecute el siguiente comando:

   ```bash
   make
   ```

En caso de que desee limpiar los archivos generados (incluido el fichero de salida de las instancias), ejecute:

   ```bash
   make reset
   ```

---

## **EJECUTAR**

Para ejecutar el programa, siga estas instrucciones:

1. Ejecute el siguiente comando:

   ```bash
   ./program input.txt
   ```

   Donde `input.txt` es el nombre del archivo de entrada que contiene el laberinto a resolver. Este archivo debe estar en el mismo directorio o en un directorio accesible sin especificar rutas adicionales.

---

## **CORRECTA VISUALIZACIÓN DE LOS RESULTADOS**

El programa genera dos tipos de resultados:

1. **Dibujo del laberinto resuelto**: Este está diseñado para ser visualizado en un editor de texto simple.
2. **Tabla de resultados**: Se recomienda utilizar un visor de markdown para una correcta visualización de este apartado.

Algunos visores de markdown recomendados incluyen:

- **Visual Studio Code**: Extensión integrada para previsualizar archivos markdown.
- **Typora**: Un editor ligero con previsualización en tiempo real.
- **MarkText**: Software gratuito y de código abierto para visualizar markdown.

---

## **ESTRUCTURA DEL PROYECTO**

- **src/**: Contiene el código fuente del programa.
- **build/**: Directorio donde se generan los archivos objeto.
- **inputs/**: Directorio que contiene los archivos de entrada del laberinto.
- **output/**: Directorio donde se almacenan los resultados generados en formato markdown.
- **include/**: Contiene los archivos de cabecera del programa.