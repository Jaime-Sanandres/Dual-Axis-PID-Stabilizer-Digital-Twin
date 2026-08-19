import serial
import time
from vpython import *

# --- 1. Serial Communication Setup ---
# Update 'COM3' (Windows) or '/dev/ttyUSB0' (Linux/Mac) to match your Arduino/Microcontroller port
# Make sure the baudrate (115200) matches the one defined in your hardware code
try:
    arduino_serial = serial.Serial('COM3', 115200, timeout=1)
    time.sleep(2)  # Allow time for the serial connection to initialize
    print("Serial connection established.")
except Exception as e:
    print(f"Error connecting to serial port: {e}")
    print("Running in simulation mode (no live data).")
    arduino_serial = None

# --- 2. VPython 3D Object Setup ---
# Create a 3D object representing a drone or platform before the loop
platform = box(length=5, width=5, height=0.5, color=color.blue)

# Optional: Add axes for better visual reference
arrow(pos=vector(0,0,0), axis=vector(3,0,0), color=color.red, shaftwidth=0.1)   # X axis
arrow(pos=vector(0,0,0), axis=vector(0,3,0), color=color.green, shaftwidth=0.1) # Y axis
arrow(pos=vector(0,0,0), axis=vector(0,0,3), color=color.blue, shaftwidth=0.1)  # Z axis

print("Starting 3D Digital Twin visualization...")

# --- 3. Main Loop ---
while True:
    # Set FPS to 50 to avoid CPU saturation and keep the visualization smooth
    rate(50)
    
    try:
        # Check if there is data waiting in the serial buffer
        if arduino_serial and arduino_serial.in_waiting > 0:
            # Read the line, decode it, and remove extra whitespace/newlines
            serial_data = arduino_serial.readline().decode('utf-8').strip()
            
            # Assuming the hardware sends data in the format: "roll,pitch" (e.g., "12.5,-4.2")
            if "," in serial_data:
                data_parts = serial_data.split(",")
                roll_pc = float(data_parts[0])
                pitch_pc = float(data_parts[1])
                
                # Convert the received degrees to radians
                roll_rad = radians(roll_pc)
                pitch_rad = radians(pitch_pc)
                
                # Update the "Up" vector using trigonometry to synchronize rotation
                platform.up = vector(
                    sin(roll_rad), 
                    cos(pitch_rad) * cos(roll_rad), 
                    -sin(pitch_rad) * cos(roll_rad)
                )
                
    except ValueError:
        # Ignore corrupted serial data (e.g., incomplete strings during startup)
        pass
    except KeyboardInterrupt:
        # Handle manual exit (Ctrl+C)
        print("Visualization stopped.")
        break
