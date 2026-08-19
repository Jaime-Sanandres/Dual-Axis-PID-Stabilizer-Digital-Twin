// --- Global Variables ---

// PID tuning parameters for one axis (Starting with Proportional only)
float Kp = 1.5;
float Ki = 0.0;
float Kd = 0.0;

// State variables to remember values between cycles
float previousErrorX = 0.0;
float integralX = 0.0;

// The target angle (perfectly flat)
float setpointX = 0.0; 


// --- PID Function ---

// Call this function inside your main loop.
// It requires the current angle (filteredAngleX) and the time step (dt) in seconds.
float calculatePID_X(float filteredAngleX, float dt) {
    
    // 1. Calculate the current error
    float errorX = setpointX - filteredAngleX;
    
    // 2. Calculate the Proportional term
    float P_x = Kp * errorX;
    
    // 3. Calculate the Integral term
    integralX = integralX + (errorX * dt);
    float I_x = Ki * integralX;
    
    // 4. Calculate the Derivative term
    // (Ensure dt > 0 to avoid division by zero errors in a real scenario)
    float derivativeX = (errorX - previousErrorX) / dt;
    float D_x = Kd * derivativeX;
    
    // 5. Sum the control signal
    float outputX = P_x + I_x + D_x;
    
    // 6. Update the error for the next cycle
    previousErrorX = errorX;
    
    // Return the final control signal to be sent to the motors
    return outputX;
}