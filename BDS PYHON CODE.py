import serial.tools.list_ports
import serial
import time
import csv
# List available COM ports
ports = list(serial.tools.list_ports.comports())

# Initialize a serial connection
serialInst = serial.Serial()

# Create a list of available ports for user selection
portsList = []

for onePort in ports:
    portsList.append(str(onePort))
    print(str(onePort))

# Ask the user to select the COM port
val = input("Enter the COM port number (e.g., 13 for COM13): ")
portVar = None

for port in ports:
    if f'COM{val}' in port.device:
        portVar = port.device
        break

if portVar is None:
    print(f"COM{val} not found.")
else:
    # Open the selected COM port
    serialInst.baudrate = 9600
    serialInst.port = portVar
    serialInst.open()

    try:
        while True:
            # Ask the user to enter a command for the Arduino
            command = input("Enter a command (R for data transfer, D for clear, B for reset, or exit to quit): ")
            serialInst.write(command.encode("utf-8"))
            
            if command.lower() == 'exit':
                break

            if command.lower() == 'r':
                # Create a CSV file on the desktop for data storage
                desktop_path ="C:\\Users\\krish\\Desktop\\IMUDATA.csv"
                with open(desktop_path, 'w', newline='') as csvfile:
                    data_writer = csv.writer(csvfile)
                    data_writer.writerow(["acc_x", "acc_y", "acc_z", "gyro_x", "gyro_y", "gyro_z", "mag_x", "mag_y", "mag_z"])


                        
                    while True:
                        # Read a line of data from the Arduino
                        arduino_data = serialInst.readline().decode().strip()

                        # Check if the Arduino has finished sending data
                        if arduino_data == "Data transfer stopped.":
                            print("Data transfer completed.")
                            break

                        # Write the data to the CSV file
                        csvfile.write(arduino_data + '\n')

                        # Print the received data (optional)
                        print(arduino_data)

                # Prompt for a new command
                continue

            if command.lower() == 'd':
                print("Cleared data on SD card.")
                # Prompt for a new command
                continue

            if command.lower() == 'b':
                print("Resetting the Arduino...")
                # Prompt for a new command
                continue

    except KeyboardInterrupt:
        print("Data transfer stopped by the user.")
    finally:
        # Close the serial port
        serialInst.close()
