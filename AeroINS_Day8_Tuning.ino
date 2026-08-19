#include <Wire.h>
#include <Servo.h>
#include <math.h>

// --- HARDWARE CONFIGURATION ---
const int ROLL_SERVO_PIN = 10;  // X-Axis (Corrected to Pin 10 from Day 1)
const int PITCH_SERVO_PIN = 9;  // Y-Axis (Corrected to Pin 9 from Day 1)
const int MPU_ADDR = 0x68;      // I2C address of the MPU-6050

Servo rollServo;
Servo pitchServo;

// --- PID TUNING PARAMETERS ---
// Start tuning here according to Day 8 instructions:
// X-Axis (Roll)
float kp_roll = 0.5; 
float ki_roll = 0.0;
float kd_roll = 0.0;

// Y-Axis (Pitch)
float kp_pitch = 0.5; 
float ki_pitch = 0.0;
float kd_pitch = 0.0;

// --- PID AND SENSOR STATE VARIABLES ---
float filtered_roll = 0.0;
float filtered_pitch = 0.0;

float roll_error = 0, roll_prev_error = 0, roll_integral = 0;
float pitch_error = 0, pitch_prev_error = 0, pitch_integral = 0;

unsigned long previous_time = 0;

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C and wake up MPU6050
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // Power management register
  Wire.write(0x00); // Wake up
  Wire.endTransmission(true);

  // Initialize Servos
  rollServo.attach(ROLL_SERVO_PIN);
  pitchServo.attach(PITCH_SERVO_PIN);
  
  // Set servos to absolute mechanical center
  rollServo.write(90);
  pitchServo.write(90);
  delay(500); // Give them time to reach the center
  
  previous_time = millis();
}

void loop() {
  // 1. Calculate time differential (dt)
  unsigned long current_time = millis();
  float dt = (current_time - previous_time) / 1000.0; // dt in seconds
  previous_time = current_time;

  if (dt <= 0.0) return; // Prevent division by zero

  // ---------------------------------------------------------
  // 2. READ IMU DATA & SENSOR FUSION (Days 2, 3, 4 & 5)
  // ---------------------------------------------------------
  
  // Read Accelerometer (0x3B)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  int16_t accX = Wire.read() << 8 | Wire.read();
  int16_t accY = Wire.read() << 8 | Wire.read();
  int16_t accZ = Wire.read() << 8 | Wire.read();

  // Convert raw accel to 'g' and calculate angles
  float gX = accX / 16384.0;
  float gY = accY / 16384.0;
  float gZ = accZ / 16384.0;
  
  float pitch_acc = atan2(gY, sqrt(gX * gX + gZ * gZ)) * 180.0 / M_PI;
  float roll_acc = atan2(-gX, gZ) * 180.0 / M_PI;

  // Read Gyroscope (0x43)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  int16_t gyroX = Wire.read() << 8 | Wire.read();
  int16_t gyroY = Wire.read() << 8 | Wire.read();
  int16_t gyroZ = Wire.read() << 8 | Wire.read();

  // Convert raw gyro to degrees per second (dps)
  float dpsX = gyroX / 131.0;
  float dpsY = gyroY / 131.0;

  // Complementary Filter: 96% Gyro, 4% Accel
  filtered_roll = 0.96 * (filtered_roll + dpsX * dt) + 0.04 * roll_acc;
  filtered_pitch = 0.96 * (filtered_pitch + dpsY * dt) + 0.04 * pitch_acc;

  // ---------------------------------------------------------
  // 3. PID CONTROLLER CALCULATIONS (Day 6)
  // ---------------------------------------------------------
  
  // Target angle is perfectly flat (0.0 degrees)
  float roll_output = calculatePID(filtered_roll, 0.0, kp_roll, ki_roll, kd_roll, roll_error, roll_prev_error, roll_integral, dt);
  float pitch_output = calculatePID(filtered_pitch, 0.0, kp_pitch, ki_pitch, kd_pitch, pitch_error, pitch_prev_error, pitch_integral, dt);

  // ---------------------------------------------------------
  // 4. ACTUATION & SATURATION LIMITS (Day 7)
  // ---------------------------------------------------------
  
  // Map output to servo center (90) and constrain to safe limits (20-160)
  int roll_servo_pos = constrain(90 + roll_output, 20, 160);
  int pitch_servo_pos = constrain(90 + pitch_output, 20, 160); // Assuming 90+ works for your mechanical setup, change to 90- if reversed

  // Write to Servos
  rollServo.write(roll_servo_pos);
  pitchServo.write(pitch_servo_pos);

  delay(10); // Loop stabilization
}

// --- PID HELPER FUNCTION ---
float calculatePID(float current_angle, float target_angle, float kp, float ki, float kd, float &error, float &prev_error, float &integral, float dt) {
  // 1. Proportional
  error = target_angle - current_angle;
  float p_term = kp * error;
  
  // 2. Integral (with anti-windup constraint)
  integral += (error * dt);
  integral = constrain(integral, -50.0, 50.0); 
  float i_term = ki * integral;
  
  // 3. Derivative
  float derivative = (error - prev_error) / dt;
  float d_term = kd * derivative;
  
  // Save error for next cycle
  prev_error = error;
  
  // Return sum of control signals
  return p_term + i_term + d_term;
}
