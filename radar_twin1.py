import serial

# Configure the serial port. 
# Change 'COM3' to your actual port (e.g., 'COM4' on Windows or '/dev/ttyUSB0' on Linux/Mac)
port = serial.Serial('COM3', 115200)

print("Starting telemetry stream... Press Ctrl+C to stop.")

try:
    while True:
        # Read the incoming line, decode to string, and remove trailing characters (\r\n)
        line = port.readline().decode('utf-8').strip()
        
        # Verify the line is not empty to avoid errors
        if line:
            # Split the comma-separated values
            data = line.split(',')
            
            # Ensure exactly two values were received before parsing
            if len(data) == 2:
                roll_pc = float(data[0])
                pitch_pc = float(data[1])
                
                # Print the data to verify real-time reception without latency
                print(roll_pc, pitch_pc)

except KeyboardInterrupt:
    # Gracefully close the connection when stopping the script
    print("\nStopping telemetry and closing port.")
    port.close()
except ValueError:
    print("Error parsing data. Ensure Arduino is sending clean float values.")
    port.close()
