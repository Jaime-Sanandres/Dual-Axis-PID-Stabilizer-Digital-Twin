#include <math.h>

void setup() {
  Serial.begin(115200);
  // Your sensor initialization code goes here...
}

void loop() {
  // Assuming accX, accY, and accZ are already read from your sensor here
  // int16_t accX = ... 
  // int16_t accY = ...
  // int16_t accZ = ...

  // 1. Transform raw data to 'g' units (assuming +/- 2g range where sensitivity is 16384 LSB/g)
  float gX = accX / 16384.0;
  float gY = accY / 16384.0;
  float gZ = accZ / 16384.0;

  // 2. Calculate Pitch (front/back tilt)
  float pitch = atan2(gY, sqrt(gX * gX + gZ * gZ)) * 180.0 / M_PI;

  // 3. Calculate Roll (side-to-side tilt)
  float roll = atan2(-gX, gZ) * 180.0 / M_PI;

  // 4. Print the angles to the Serial monitor
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print(" \t Roll: ");
  Serial.println(roll);

  // Small delay to make the Serial Monitor readable
  delay(100); 
}