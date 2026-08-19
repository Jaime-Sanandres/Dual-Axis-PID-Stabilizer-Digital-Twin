// --- Global Variables ---
// These should be placed at the top of your sketch, before setup()
float filteredAngleX = 0;
float filteredAngleY = 0;

void setup() {
  Serial.begin(115200);
  // (Your previous IMU initialization code goes here)
}

void loop() {
  // ---------------------------------------------------------
  // 1. PREVIOUS CALCULATIONS (From Days 1-4)
  // ---------------------------------------------------------
  // Make sure you have calculated the following variables 
  // before applying the filter:
  // float dt = ...;        // Delta time in seconds
  // float dpsX = ...;      // Gyroscope rate X (degrees per second)
  // float dpsY = ...;      // Gyroscope rate Y (degrees per second)
  // float roll = ...;      // Accelerometer angle X
  // float pitch = ...;     // Accelerometer angle Y
  
  // ---------------------------------------------------------
  // 2. DAY 5: SENSOR FUSION (COMPLEMENTARY FILTER)
  // ---------------------------------------------------------
  
  // Apply the complementary filter equation for Roll (X-Axis)
  // 96% Trust in Gyroscope (short-term stability)
  // 4% Trust in Accelerometer (long-term absolute reference)
  filteredAngleX = 0.96 * (filteredAngleX + dpsX * dt) + 0.04 * roll;
  
  // Apply the same complementary filter equation for Pitch (Y-Axis)
  filteredAngleY = 0.96 * (filteredAngleY + dpsY * dt) + 0.04 * pitch;

  // ---------------------------------------------------------
  // 3. SERIAL PLOTTER OUTPUT
  // ---------------------------------------------------------
  
  // Print ONLY the filtered variables for the Serial Plotter.
  // The comma separates the values so the plotter draws two distinct lines.
  Serial.print("Filtered_Roll_X:");
  Serial.print(filteredAngleX);
  Serial.print(",");
  Serial.print("Filtered_Pitch_Y:");
  Serial.println(filteredAngleY);

  // A small delay to keep the loop rate stable 
  // (adjust based on your previous code)
  delay(10); 
}