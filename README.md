# Dual-Axis PID Stabilizer & 3D Digital Twin

![Project Status](https://img.shields.io/badge/Status-Completed-success)
![Hardware](https://img.shields.io/badge/Hardware-Arduino_Uno-blue)
![Python](https://img.shields.io/badge/Python-3.10%2B-yellow)

> **Note:** [Insert an image or GIF here showing the physical platform moving alongside the 3D Digital Twin]

A complete hardware and software engineering project that actively stabilizes a 2-axis platform (Pan-Tilt) using an MPU6050 sensor, and streams real-time telemetry to a 3D Digital Twin running on a PC. 

This project was built from scratch without relying on high-level sensor libraries, focusing heavily on **bare-metal communication, sensor fusion, and control theory**.

## 🧠 Core Features & Engineering Concepts

*   **Bare-Metal I2C Communication:** Direct register reading of the MPU6050 accelerometer and gyroscope without standard abstraction libraries.
*   **Spatial Trigonometry:** Conversion of raw digital voltages into absolute Pitch and Roll angles.
*   **Sensor Fusion (Complementary Filter):** Combines the short-term precision of the gyroscope (integration over time) with the long-term absolute reference of the accelerometer to eliminate drift and mechanical noise.
*   **Custom PID Controller:** Industrial standard Proportional-Integral-Derivative algorithm implemented from scratch to calculate the exact error and apply the correct PWM signal to the servos.
*   **Data Pipeline & Digital Twin:** Serial telemetry streaming from C++ to Python, parsed and visualized in a 3D environment using VPython at 50 FPS.

## 🛠️ Hardware & Tech Stack

**Hardware:**
*   1x Arduino Uno
*   1x MPU6050 (6-DoF IMU)
*   2x SG90 Micro Servomotors (Pitch and Roll axes)
*   Custom Pan-Tilt structure
*   Jumper wires & Breadboard

**Software:**
*   **C++ (Arduino IDE):** Embedded logic, bare-metal I2C protocol implementation, math algorithms, and standard `Servo.h` library for PWM signal generation.
*   **Python 3:** `pyserial` for data ingestion, `vpython` for the 3D real-time rendering.

## 🔌 Wiring Guide

| MPU6050 Pin | Arduino Pin | Description |
| :--- | :--- | :--- |
| VCC | 5V / 3.3V | Power (check module regulator) |
| GND | GND | Ground |
| SDA | A4 | I2C Data |
| SCL | A5 | I2C Clock |

*   **Servo X (Roll):** Signal to Digital Pin 10, VCC to 5V, GND to GND.
*   **Servo Y (Pitch):** Signal to Digital Pin 9, VCC to 5V, GND to GND.

## 🚀 How to Run the Project

### 1. Arduino Setup (Hardware)
1. Assemble the hardware and wire it according to the table above.
2. Open the `arduino_src` folder and upload the `.ino` code to your Arduino Uno.
3. Once powered, keep the base completely flat. The servos will initialize at 90 degrees (mechanical zero), and the PID controller will start keeping the platform stable against disturbances.

### 2. Python Setup (Digital Twin)
1. Ensure Python 3.10+ is installed and added to your PATH.
2. Install the required Python dependencies by running this in your terminal:
   ```bash
   pip install pyserial vpython
   ```
3. Open `radar_twin.py` in the `python_twin` folder.
4. Modify the serial port line to match your Arduino's port (e.g., `COM3` for Windows or `/dev/ttyUSB0` for Linux/Mac):
   ```python
   puerto = serial.Serial('COM3', 115200)
   ```
5. Run the Python script. A browser window will open displaying the 3D model reacting in real-time to the physical movements of your sensor.

## 📈 Tuning the PID

If you build your own physical structure, you will need to tune the PID gains due to different mass distributions. Inside the Arduino code, look for these variables:

```cpp
float Kp = 1.5;  // Proportional: Reacts to current error
float Ki = 0.01; // Integral: Corrects tiny steady-state errors
float Kd = 0.05; // Derivative: Dampens the movement, preventing overshooting
```

*Adjust these carefully. Too much `Kp` causes aggressive oscillations, too much `Kd` makes the system sluggish.*

## 🤝 Contact & Connect
Created by Jaime Sanandrés. Feel free to reach out on [LinkedIn](https://www.linkedin.com/in/jaime-sanandr%C3%A9s-aa6b93308/?locale=en-US) if you have any questions about the control theory or the implementation!
