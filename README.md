# Sistema de Riego Automatizado Basado en IoT para Cultivo de Olivar (TRL 4)

Repositorio oficial del proyecto de ingeniería enfocado en el diseño, desarrollo y validación mediante simulación computacional de un nodo de riego inteligente orientado a explotaciones de olivar en **Santa Cruz de Mudela (Ciudad Real, España)**.

## 📋 Descripción del Proyecto
El sistema implementa una arquitectura IoT modular estructurada en capas para optimizar el recurso hídrico y reducir el consumo energético frente a los métodos de riego empíricos tradicionales. El proyecto alcanza un nivel de madurez tecnológica **TRL 4** (validación de componentes en entorno simulado), integrando sensórica capacitiva, procesamiento local (*Edge Computing*) en un microcontrolador **ESP32**, y un sistema de control automático en lazo cerrado basado en el modelo agronómico FAO-56.

---

## 🏗️ Arquitectura del Sistema
El sistema se organiza en cuatro capas funcionales principales:
1. **Capa de Percepción:** Sensor capacitivo de humedad del suelo (%VWC) y simulación analógica de variables agronómicas.
2. **Capa de Procesamiento (Edge Computing):** Microcontrolador ESP32 ejecutando la lógica de control local.
3. **Capa de Red y Comunicaciones:** Transmisión de telemetría mediante el protocolo ligero **MQTT**.
4. **Capa de Actuación y Aplicación:** Módulo relé de un canal conectado a una electroválvula y panel de visualización indicador.

---

## 🔌 Asignación de Pines (GPIO Mapping)
| Componente | Pin del Componente | Pin del ESP32 | Tipo de Señal / Función |
| :--- | :--- | :--- | :--- |
| **Sensor Capacitivo (%VWC)** | VCC / GND / AOUT | 3.3V / GND / GPIO 34 | Alimentación y lectura analógica (ADC1_CH6) |
| **Módulo Relé (Actuador)** | VCC / GND / IN | 5V / GND / GPIO 25 | Alimentación y salida digital de control (ON/OFF) |
| **Circuito LED Indicador** | Anodo / Cátodo | GPIO 14 / GND | Indicador visual de potencia y conmutación de carga en contacto NO |

---

## 💻 Firmware y Lógica de Control (C/C++)
El firmware implementa un bucle de control iterativo en lazo cerrado con una estructura de tres estados basada en umbrales agronómicos discretos ($H_{\min} = 1200$ y $H_{\max} = 3000$ en escala ADC de 12 bits):

```cpp
void loop() {
  // 1. Leer el valor analógico del sensor de humedad (simulado con potenciómetro en Wokwi)
  int valorSensor = analogRead(SENSOR_PIN);
  
  // 2. Convertir a un porcentaje estimado (0% a 100%) para el monitor serial
  float porcentajeHumedad = (float)valorSensor / 4095.0 * 100.0;

  // 3. Imprimir registros de ejecución
  Serial.print("Lectura ADC (GPIO 34): ");
  Serial.print(valorSensor);
  Serial.print(" | Humedad estimada: ");
  Serial.print(porcentajeHumedad);
  Serial.println("%");

  // 4. Lógica de control en lazo cerrado con zona intermedia definida
  if (valorSensor < UMBRAL_SEQUEADAD) {
    // Suelo seco: Activar riego
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    Serial.println("-> ESTADO: Suelo seco detectado. Valvula ABIERTA (Riego Activo).");
  } 
  else if (valorSensor > UMBRAL_HUMEDAD) {
    // Suelo húmedo: Desactivar riego
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    Serial.println("-> ESTADO: Humedad optima. Valvula CERRADA (Riego Inactivo).");
  }
  else {
    // Zona de histéresis o transición
    Serial.println("-> ESTADO: Humedad en rango de transicion (Esperando umbrales).");
  } 
}
