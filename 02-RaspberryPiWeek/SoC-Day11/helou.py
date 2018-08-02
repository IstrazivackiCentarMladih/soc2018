#!/usr/bin/env python3

# pip3 install pyserial
# sudo apt install arduino

import serial
import time

# ls /dev | grep tty
# ili dmesg
ARDUINO_DEVICE = "/dev/ttyUSB0"
BAUD_RATE = 9600
DEVICE_WAIT = 2

arduino = serial.Serial(ARDUINO_DEVICE, BAUD_RATE, timeout=None)
time.sleep(DEVICE_WAIT)

#Serial.in_waiting


while True:
    arduino.write(b'0')
    
    while arduino.in_waiting != 2: # equivalent to Serial.available()
        pass 
    
    print(arduino.read_all())
    
    time.sleep(1)


    arduino.write("1".encode())


    print(arduino.read(2).decode())       
    time.sleep(1)


arduino.close()
