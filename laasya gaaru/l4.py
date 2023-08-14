import serial
import time
import os
import csv

# Define the image dimensions (width and height)
image_width = 320
image_height = 240

# Establish a serial connection
ser = serial.Serial('COM24', 9600, timeout=1)  # Replace 'COM24' with the actual COM port
print("Serial port opened.")

try:
    # Send the "capture" command to the Arduino
    ser.write(b"capture\n")

    # Wait for a brief period to allow the Arduino to start data capture
    time.sleep(1)

    # Create a CSV file to store the data
    with open('output_data.csv', 'w', newline='') as csvfile:
        csv_writer = csv.writer(csvfile)

        # Continuously read and save the data until the "stop" command is sent
        while True:
            # Read the raw data from the serial port
            raw_data = ser.readline().decode('utf-8').strip()

            # Stop capturing data if the "stop" command is received
            if raw_data == "Data capture stopped.":
                print("Data capture stopped by the Arduino.")
                break

            # Split the raw data into individual values (assuming space-separated values)
            values = raw_data.split(' ')

            # Write the values to the CSV file
            csv_writer.writerow(values)

except KeyboardInterrupt:
    # Send the "stop" command to the Arduino if the script is interrupted
    ser.write(b"stop\n")
    print("Sent 'stop' command to the Arduino.")

finally:
    # Close the serial port
    ser.close()
    print("Serial port closed.")
