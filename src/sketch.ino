/*
 * Proyecto: Sistema de Riego Automatizado IoT - Olivar (TRL 4)
 * Controlador: ESP32
 * Descripción: Lectura analógica de humedad (Sensor en GPIO 34) 
 * y control automático de electroválvula/relé (GPIO 25) en lazo cerrado.
 */

// Definición de pines según la arquitectura del hardware
const int SENSOR_PIN = 34;   // Pin analógico ADC1 para el sensor de humedad
const int RELAY_PIN = 25;    // Pin digital para activar el módulo relé
const int LED_PIN = 4;       // Pin opcional para indicador visual de riego activo

// Umbrales de humedad (% VWC / Calibración simulada de 0 a 4095)
const int UMBRAL_SEQUEADAD = 1200; // Por debajo de este valor, el suelo está seco -> Riega
const int UMBRAL_HUMEDAD = 3000;   // Por encima de este valor, el suelo está húmedo -> Detiene riego

void setup() {
  // Inicialización de la comunicación serial para monitorización (Evidencia clave para el tutor)
  Serial.begin(115200);
  delay(1000);
  Serial.println("--- Sistema de Riego IoT Iniciado ---");

  // Configuración de modos de pines
  pinMode(SENSOR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Asegurar estado inicial seguro (Relé apagado)
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // 1. Leer el valor analógico del sensor de humedad (simulado con potenciómetro en Wokwi)
  int valorSensor = analogRead(SENSOR_PIN);
  
  // 2. Convertir opcionalmente a un porcentaje estimado (0% a 100%) para el monitor serial
  float porcentajeHumedad = (float)valorSensor / 4095.0 * 100.0;

  // 3. Imprimir registros de ejecución (Evidencia requerida por el tutor)
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
    // Suelo humedo: Desactivar riego
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    Serial.println("-> ESTADO: Humedad optima. Valvula CERRADA (Riego Inactivo).");
  }
  else {
    // Zona de histéresis o transición (mantiene el estado anterior o indica espera)
    Serial.println("-> ESTADO: Humedad en rango de transicion (Esperando umbrales).");
  }

  // Retardo para estabilizar lecturas en la simulación
  delay(2000); 
}