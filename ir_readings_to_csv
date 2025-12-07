import serial
import csv
import time
import os

PORT = "COM15"
BAUD = 9600

ser = serial.Serial(PORT, BAUD)
time.sleep(2)

filename = r"C:\Users\ashutosh sriram\OneDrive\Desktop\ppg_raw.csv"
print("Saving to:", filename)

csvfile = open(filename, "w", newline="")
writer = csv.writer(csvfile)

print("Recording for 60 seconds...")

start_time = time.time()

try:
    while True:
        # Stop after 60 sec
        if time.time() - start_time >= 60:
            print("60 seconds completed.")
            break

        line = ser.readline().decode(errors="ignore").strip()

        try:
            value = int(line)
            writer.writerow([value])
            csvfile.flush()
            print(value)
        except ValueError:
            pass

except KeyboardInterrupt:
    print("Stopped manually.")

# Cleanup
csvfile.close()
ser.close()

print("Saved:", filename)
