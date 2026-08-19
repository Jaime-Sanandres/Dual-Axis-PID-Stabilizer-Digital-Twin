#include <Servo.h>

// Create servo objects to control the servos
Servo servoX; // Controls the X-Axis / Roll
Servo servoY; // Controls the Y-Axis / Pitch

void setup() {
  // Assign the control pins for each servo
  servoX.attach(10); // Orange wire of Servo 2 to digital pin 10
  servoY.attach(9);  // Orange wire of Servo 1 to digital pin 9
  
  // Set both motors to their absolute mechanical center (90 degrees)
  servoX.write(90);
  servoY.write(90);
  
  // Optional: Short delay to allow the motors to reach their position
  delay(500);
}

void loop() {
  // For this calibration phase, we don't need the code to do anything else.
  // Once setup() runs, the motors will hold their 90-degree position.
}