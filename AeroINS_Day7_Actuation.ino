#include <Servo.h>

// Create servo object to control the motor
Servo servoX;

// Define mechanical center and saturation limits
const int SERVO_CENTER = 90;
const int SERVO_MAX = 160;
const int SERVO_MIN = 20;

void setup() {
  // Attach the servo on pin 9 (adjust pin number as needed for your wiring)
  servoX.attach(9);
  
  // Initialize the platform at the mechanical center
  servoX.write(SERVO_CENTER);
}

void loop() {
  // ---------------------------------------------------------
  // [!] Your previous IMU reading and PID math should go here
  // to calculate the 'outputX' variable.
  // ---------------------------------------------------------
  
  float outputX = 0; // Placeholder: Replace this with your actual PID output variable

  // 1. Translate PID math to PWM signal by applying it to the base position
  int servoPositionX = SERVO_CENTER + outputX;

  // 2. Implement critical constraints (Saturation) to prevent jerky movements and motor damage
  if (servoPositionX > SERVO_MAX) {
    servoPositionX = SERVO_MAX;
  }
  
  if (servoPositionX < SERVO_MIN) {
    servoPositionX = SERVO_MIN;
  }

  // 3. Send the command to the motor to close the control loop
  servoX.write(servoPositionX);
  
  // Small delay to allow the servo to reach the position
  delay(15); 
}
