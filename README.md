# Simulador de Planta de Tratamiento de Aguas Residuales (WWTP Simulator)

Este proyecto es un simulador interactivo que modela el funcionamiento de una planta de tratamiento de aguas residuales, o al menos la unión de sus partes, el proyecto está siendo diseñado para estudiantes, ingenieros y profesionales interesados en el tratamiento de aguas. La aplicación permite construir sistemas personalizados, simular flujos de agua y analizar los cambios en parámetros clave como **BOD (Demanda Bioquímica de Oxígeno)**, **COD (Demanda Química de Oxígeno)**, **TSS (Sólidos Suspendidos Totales)**, **NH₄⁺ (Amonio)**, entre otros. Es decir, se puede aplicar una serie de componentes y sus procesos para luego ver la diferencia entre inlet y outlet de agua y sus parámetros.

## Características

### 1. Construcción de Sistemas
- **Elementos Disponibles:** Sedimentadores, tanques aeróbicos/anaeróbicos, filtros, unidades de desinfección, bombas y más.
- **Parámetros Personalizables:** Flujo, volumen, tiempos de retención hidráulica (HRT) y sólidos (SRT), temperatura, etc.
- **Conexión de Componentes:** Permite conectar unidades para crear sistemas personalizados.

### 2. Simulación Dinámica
- **Visualización en Tiempo Real:** Se está construyendo una representación gráfica del flujo de agua y parámetros en cada unidad del sistema.
- **Modelos Realistas:** Basado en ecuaciones de ingeniería ambiental estándar.
- **Cambios en Parámetros:** Simulación de remoción de contaminantes (BOD, COD, TSS, etc.) y procesos como nitrificación/desnitrificación.

### 3. Interfaz Gráfica Interactiva
- **Integración con ImGui:** Controles para personalizar parámetros, iniciar/pausar simulaciones, y agregar componentes.
- **Gráficos Dinámicos:** Monitoreo en tiempo real de los parámetros de entrada y salida.
- **Modo de Ejemplo:** Configuración predeterminada para explorar rápidamente un sistema funcional.

## Tecnologías Utilizadas
- **Lenguaje:** C++ (Estándar C++17).
- **Bibliotecas Gráficas:** SFML para renderización gráfica.
- **Interfaz de Usuario:** ImGui integrada con SFML.
- **Diseño Modular:** Arquitectura orientada a objetos para modelar componentes.

## PLAN A FUTURO ES IMPLEMENTAR UNA VISUALIZACIÓN 3D CON UNA SIMULACIÓN DE FLUJOS MÁS REAL Y ESCALABLE EN ESPACIO Y TIEMPO.

## Cómo Ejecutarlo
1. Asegúrate de tener las bibliotecas necesarias:
   - SFML.
   - ImGui-SFML.
2. Compila el proyecto con el siguiente comando (reemplaza las rutas según tu sistema):
   ```bash
   g++ -std=c++17 -o wwtp_simulator main6.cpp -I<ruta_a_sfml> -L<ruta_a_librerias> -lsfml-graphics -lsfml-window -lsfml-system
