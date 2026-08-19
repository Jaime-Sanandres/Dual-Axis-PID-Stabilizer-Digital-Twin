#include <Wire.h>

// MPU6050 I2C address
const uint8_t MPU6050_ADDR = 0x68;

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);
  
  // Initialize I2C bus
  Wire.begin();
  
  // Wake up the MPU6050 sensor
  // Access the power management register (0x6B) and write 0x00 to wake it up
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B); 
  Wire.write(0x00); 
  Wire.endTransmission(true);
}

void loop() {
  // Begin transmission to the sensor
  Wire.beginTransmission(MPU6050_ADDR);
  
  // Point to the first accelerometer data register (0x3B)
  Wire.write(0x3B);
  
  // End transmission but keep the connection active
  Wire.endTransmission(false); 
  
  // Request 6 bytes of data from the sensor (2 bytes for each axis: X, Y, Z)
  Wire.requestFrom((int)MPU6050_ADDR, 6, (int)true);
  
  // Read and reconstruct the 16-bit raw accelerometer values
  // Shift the first byte 8 bits to the left and combine with the second byte using bitwise OR
  int16_t accX = (Wire.read() << 8) | Wire.read();
  int16_t accY = (Wire.read() << 8) | Wire.read();
  int16_t accZ = (Wire.read() << 8) | Wire.read();
  
  // Print the raw values formatted for the Arduino Serial Plotter
  Serial.print("AccX:");
  Serial.print(accX);
  Serial.print(",");
  Serial.print("AccY:");
  Serial.print(accY);
  Serial.print(",");
  Serial.print("AccZ:");
  Serial.println(accZ);
  
  // Short delay to stabilize the Serial Plotter reading
  delay(50);
}
