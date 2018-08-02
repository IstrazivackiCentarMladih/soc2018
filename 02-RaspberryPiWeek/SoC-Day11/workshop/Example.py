#!/usr/bin/env python3

import serial
import time

ARDUINO_DEVICE = "/dev/ttyUSB0" #  win: "COM3" ili tak nesto
BAUD_RATE = 2000000


moj_arduino = serial.Serial(ARDUINO_DEVICE, BAUD_RATE)
time.sleep(2) # inicijalizacija


while True:
    moj_arduino.write("1".encode())
    
    #while moj_arduino.in_waiting < 16:
    #    pass # cekamo na 16 znakova nase povratne recenice: "LED-ica upaljena" 
    
    print(moj_arduino.read(16).decode())
    time.sleep(1)
    
    moj_arduino.write("0".encode())
    #while moj_arduino.in_waiting < 15:
    #    pass # cekamo na 16 znakova nase povratne recenice: "LED-ica ugasena" 
    
    print(moj_arduino.read(15).decode())
    time.sleep(1)

