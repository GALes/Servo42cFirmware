# Análisis del Hardware: MKS SERVO42C V1.0

Este documento describe los componentes principales y la funcionalidad de la placa controladora de motor paso a paso en lazo cerrado MKS SERVO42C V1.0.

## Esquema

![Esquema del MKS SERVO42C](MKS-SERVO42C-schematic.png)

Puedes descargar el esquema completo en formato PDF desde este [enlace](./MKS%20SERVO42C-schematic.pdf).

### Resumen General

Este circuito es un controlador de motor paso a paso (stepper) en lazo cerrado. Esto significa que no solo envía señales para mover el motor, sino que también utiliza un sensor de posición para verificar si el motor se ha movido a la posición deseada, corrigiendo cualquier error. Es una mejora significativa sobre los controladores de motor paso a paso estándar (como el A4988 o DRV8825) que operan en lazo abierto.

### Componentes Clave y Funcionalidad

1.  **Microcontrolador (MCU) - `U1: HC32L130F8UA`**:
    *   **Función**: Es el cerebro de la placa. Este MCU de 32 bits (ARM Cortex-M0+) ejecuta el firmware que controla toda la lógica.
    *   **Detalles**: Recibe las señales de control `Stp` (paso), `Dir` (dirección) y `En` (habilitación) desde una placa de control principal (como una placa de impresora 3D). Procesa los datos del encoder magnético (`U4`) para determinar la posición del motor y luego comanda el driver del motor (`U2`, `U3`, `U5`, `U6`) para mover el motor paso a paso de acuerdo a las señales de entrada y la retroalimentación del encoder. También maneja la comunicación para la pantalla OLED y los botones de la interfaz de usuario.

2.  **Driver de Motor (Puente-H) - `U2, U3, U5, U6: EG3013` y MOSFETs `Q4-Q11: CJ3400`**:
    *   **Función**: Esta es la sección de potencia que maneja la corriente que va al motor.
    *   **Detalles**: El `EG3013` es un "gate driver" que toma las señales lógicas de bajo voltaje del MCU y las convierte en señales de alto voltaje/corriente capaces de activar y desactivar rápidamente los MOSFETs (`CJ3400`). Los MOSFETs están dispuestos en una configuración de "Puente-H", que permite que la corriente fluya en cualquier dirección a través de las bobinas del motor, permitiendo así el control preciso de los pasos del motor. Hay dos puentes-H, uno para cada una de las dos bobinas (A y B) del motor paso a paso.

3.  **Encoder Magnético - `U4: MT6816CT-ACD`**:
    *   **Función**: Este es el sensor de posición que cierra el lazo de control.
    *   **Detalles**: Es un sensor de efecto Hall que detecta la orientación de un imán que se monta en el eje trasero del motor. El `MT6816CT` proporciona la posición angular absoluta del eje del motor al MCU a través de una interfaz SPI. Esto permite al firmware saber exactamente dónde está el motor en todo momento y corregir si pierde pasos.

4.  **Fuente de Alimentación y Reguladores**:
    *   **`U7: MD8942`**: Un regulador de voltaje "buck" (reductor) que probablemente toma el voltaje de entrada principal del motor (V+) y lo reduce a un voltaje intermedio (VC2).
    *   **`U12: CL9195A33L5M`**: Un regulador de voltaje LDO (Low-Dropout) que toma el voltaje intermedio y lo reduce a 3.3V. Este voltaje de 3.3V es el que alimenta al MCU y a la mayoría de los componentes lógicos de la placa.

5.  **Interfaz de Entrada/Salida**:
    *   **`J3`**: Conector de entrada de control. Aquí es donde se conectan las señales `Stp`, `Dir`, `En` y la alimentación desde la placa base de la impresora 3D o del controlador CNC.
    *   **`J6`**: Conector de salida para el motor paso a paso. Las 4 líneas (`A+`, `A-`, `B+`, `B-`) se conectan a las bobinas del motor.
    *   **`J2 (LCD)`**: Conector para una pantalla OLED (I2C), que permite mostrar información como la posición del motor, el estado, etc.
    *   **`J1`**: Conector para comunicación serie (UART - `TX`/`RX`), probablemente para depuración o configuración avanzada.
    *   **Botones (`Next`, `Enter`, `Menu`)**: Permiten la interacción del usuario a través de un menú en la pantalla OLED.

6.  **Circuitos de Protección**:
    *   **`D1, D2 (SMF26A, SMF5.0A)`**: Diodos de supresión de voltaje transitorio (TVS) para proteger el circuito de picos de voltaje.
    *   **`U8, U9 (EL0631, EL357N)`**: Optoacopladores. Aíslan eléctricamente las señales de entrada (`Stp`, `Dir`, `En`) del resto del circuito, lo que ayuda a prevenir que el ruido eléctrico de la placa principal afecte al controlador del motor. La nota "Only one group of Q1,Q2,Q3 and U8,U9 can be selected for welding" indica que la placa puede ser ensamblada para usar los optoacopladores o una entrada directa con MOSFETs (`Q1-Q3`), dependiendo de la configuración deseada.

### Flujo de Operación

1.  La placa de control principal envía pulsos a la entrada `Stp` (cada pulso es un paso) y una señal en `Dir` para la dirección.
2.  El MCU (`U1`) recibe estas señales (a través de los optoacopladores).
3.  El MCU lee la posición actual del motor desde el encoder magnético (`U4`).
4.  Basado en la señal de `Stp` y la posición actual, el MCU calcula cómo debe energizar las bobinas del motor para moverse un paso en la dirección correcta.
5.  El MCU envía señales PWM (Modulación por Ancho de Pulso) a los gate drivers (`EG3013`).
6.  Los gate drivers activan los MOSFETs del puente-H, enviando corriente a las bobinas del motor.
7.  El motor se mueve.
8.  El encoder magnético (`U4`) informa la nueva posición al MCU.
9.  El MCU compara la nueva posición con la posición deseada. Si hay una diferencia (un paso perdido), puede intentar corregirlo.
10. El ciclo se repite para cada pulso de `Stp`.
