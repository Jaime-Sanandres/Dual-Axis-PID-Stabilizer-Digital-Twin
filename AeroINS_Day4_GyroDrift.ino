#include <Wire.h>

const int MPU_ADDR = 0x68; // Standard I2C address for the sensor (e.g., MPU6050)

// Time tracking variables
unsigned long previousTime = 0;

// Variables to store the accumulated angles
float gyroAngleX = 0.0;
float gyroAngleY = 0.0;
float gyroAngleZ = 0.0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Wake up the sensor
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // Power management register
  Wire.write(0x00); // Set to 0 to wake the sensor up
  Wire.endTransmission(true);
}

void loop() {
  // 1. Calculate time differential (dt)
  unsigned long currentTime = millis();
  float dt = (currentTime - previousTime) / 1000.0; // Convert to seconds
  previousTime = currentTime;

  // 2. Request 6 registers starting from 0x43 (Gyroscope data)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x43); // Starting register for Gyro X
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true); 

  // 3. Read the 6 registers and combine high and low bytes using bitwise operations
  int16_t gyroX = Wire.read() << 8 | Wire.read();
  int16_t gyroY = Wire.read() << 8 | Wire.read();
  int16_t gyroZ = Wire.read() << 8 | Wire.read();

  // 4. Convert raw data to degrees per second (dps)
  float dpsX = gyroX / 131.0;
  float dpsY = gyroY / 131.0;
  float dpsZ = gyroZ / 131.0;

  // 5. Integrate velocity to find the angle
  gyroAngleX = gyroAngleX + (dpsX * dt);
  gyroAngleY = gyroAngleY + (dpsY * dt);
  gyroAngleZ = gyroAngleZ + (dpsZ * dt);

  // 6. Print gyroAngleX to Serial Monitor to observe the drift
  Serial.print("Gyro Angle X: ");
  Serial.println(gyroAngleX);
  
  // Small delay to avoid saturating the Serial output
  delay(10);
}