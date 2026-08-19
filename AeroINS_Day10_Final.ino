#include <Wire.h>
#include <Servo.h>
#include <math.h>

// --- CONFIGURACIÓN DE HARDWARE ---
const int ROLL_SERVO_PIN = 10;  // Eje X
const int PITCH_SERVO_PIN = 9;  // Eje Y
const int MPU_ADDR = 0x68;      // Dirección I2C del MPU-6050

Servo rollServo;
Servo pitchServo;

// --- PARÁMETROS PID ---
// Eje X (Roll)
float kp_roll = 0.5; 
float ki_roll = 0.01;
float kd_roll = 0.05;

// Eje Y (Pitch)
float kp_pitch = 0.5; 
float ki_pitch = 0.01;
float kd_pitch = 0.05;

// --- VARIABLES DE ESTADO PID Y SENSORES ---
float filtered_roll = 0.0;
float filtered_pitch = 0.0;

float roll_error = 0, roll_prev_error = 0, roll_integral = 0;
float pitch_error = 0, pitch_prev_error = 0, pitch_integral = 0;

unsigned long previous_time = 0;

void setup() {
  // Comunicación a 115200 baudios para que Python lo lea rápido y sin lag
  Serial.begin(115200);
  
  // Inicializar I2C y despertar el sensor
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // Registro de energía
  Wire.write(0x00); // Despertar
  Wire.endTransmission(true);

  // Inicializar Servos
  rollServo.attach(ROLL_SERVO_PIN);
  pitchServo.attach(PITCH_SERVO_PIN);
  
  // Centrado mecánico absoluto
  rollServo.write(90);
  pitchServo.write(90);
  delay(500); 
  
  previous_time = millis();
}

void loop() {
  // 1. Calcular diferencial de tiempo (dt)
  unsigned long current_time = millis();
  float dt = (current_time - previous_time) / 1000.0; 
  previous_time = current_time;

  if (dt <= 0.0) return; // Prevenir división por cero

  // ---------------------------------------------------------
  // 2. LECTURA Y FUSIÓN DE SENSORES
  // ---------------------------------------------------------
  
  // Leer Acelerómetro
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 4, true);

  int16_t accX = Wire.read() << 8 | Wire.read();
  int16_t accY = Wire.read() << 8 | Wire.read();
  int16_t accZ = Wire.read() << 8 | Wire.read();

  float gX = accX / 16384.0;
  float gY = accY / 16384.0;
  float gZ = accZ / 16384.0;
  
  float pitch_acc = atan2(gY, sqrt(gX * gX + gZ * gZ)) * 180.0 / M_PI;
  float roll_acc = atan2(-gX, gZ) * 180.0 / M_PI;

  // Leer Giroscopio
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 4, true);

  int16_t gyroX = Wire.read() << 8 | Wire.read();
  int16_t gyroY = Wire.read() << 8 | Wire.read();

  float dpsX = gyroX / 131.0;
  float dpsY = gyroY / 131.0;

  // Filtro Complementario
  filtered_roll = 0.96 * (filtered_roll + dpsX * dt) + 0.04 * roll_acc;
  filtered_pitch = 0.96 * (filtered_pitch + dpsY * dt) + 0.04 * pitch_acc;

  // ---------------------------------------------------------
  // 3. CONTROLADOR PID
  // ---------------------------------------------------------
  
  float roll_output = calculatePID(filtered_roll, 0.0, kp_roll, ki_roll, kd_roll, roll_error, roll_prev_error, roll_integral, dt);
  float pitch_output = calculatePID(filtered_pitch, 0.0, kp_pitch, ki_pitch, kd_pitch, pitch_error, pitch_prev_error, pitch_integral, dt);

  // ---------------------------------------------------------
  // 4. ACTUACIÓN Y LÍMITES
  // ---------------------------------------------------------
  
  int roll_servo_pos = constrain(90 + roll_output, 20, 160);
  int pitch_servo_pos = constrain(90 + pitch_output, 20, 160);

  rollServo.write(roll_servo_pos);
  pitchServo.write(pitch_servo_pos);

  // ---------------------------------------------------------
  // 5. TELEMETRÍA (Pipeline de Datos para Python)
  // ---------------------------------------------------------
  
  // Imprimimos EXACTAMENTE los dos valores separados por una coma, sin texto extra.
  Serial.print(filtered_roll);
  Serial.print(",");
  Serial.println(filtered_pitch);

  delay(10);
}

// --- FUNCIÓN AUXILIAR PID ---
float calculatePID(float current_angle, float target_angle, float kp, float ki, float kd, float &error, float &prev_error, float &integral, float dt) {
  error = target_angle - current_angle;
  float p_term = kp * error;
  
  integral += (error * dt);
  integral = constrain(integral, -50.0, 50.0); 
  float i_term = ki * integral;
  
  float derivative = (error - prev_error) / dt;
  float d_term = kd * derivative;
  
  prev_error = error;
  return p_term + i_term + d_term;
}