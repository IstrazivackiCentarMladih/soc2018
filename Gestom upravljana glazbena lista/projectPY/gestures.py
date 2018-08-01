# -*- coding: utf-8 -*-
"""
Created on Thu Jul 26 18:52:13 2018

@author: Luka
"""

import serial
import time
import numpy as np
# import for multiprocessing
import subprocess
# imports for filesystem
import os


ARDUINO_DEVICE = "/dev/ttyACM0"
BAUD_RATE = 115200
GESTURE_DIST = 30

gesture = "None"
stateTemp = "None"
state = "None"

arduinoSerial = serial.Serial(ARDUINO_DEVICE, BAUD_RATE)
time.sleep(2)

def toUInt16 (bytearray):
    return (bytearray[1] << 8) + bytearray[0]

def readSensors():
    readRight = arduinoSerial.read(2)
    readLeft  = arduinoSerial.read(2)

    distanceRight = toUInt16(readRight)
    distanceLeft  = toUInt16(readLeft)
    
    print("Left:  " + str(distanceLeft) + "\t | Right: " + str(distanceRight))

    return distanceLeft, distanceRight

################################################################################
#
################################################################################
def play(index):
    
    return subprocess.Popen(['mpg123', playListDir + '/' + playList[index]],
                           stdout=subprocess.DEVNULL,
                           stderr=subprocess.DEVNULL,
                           stdin=subprocess.DEVNULL)
    

# create list from given directory
playListDir = 'play_list'
playList = os.listdir(playListDir)

# playing songs from list
index = 0

# gesture global variable
#   1 left
#   2 right
#   3 playing/stop

pro = play(index)


def control_player(gesture):
    global index
    if gesture == 1:
        pro.terminate()
        if index == 0:
            index = 0
        else:
            index = index - 1
            
        play(index)
        gesture = 3
            
    elif gesture == 2:
        pro.terminate()
        if index == len(playList)-1:
            index = len(playList)-1
        else:
            index = index + 1
            
        play(index)   
        gesture = 3
        
    else:
        pass
    
 
 
 
 
################################################################################
#
################################################################################
 
# Start communication
arduinoSerial.write("1".encode())

while True:
    
    distanceLeft, distanceRight = readSensors()

    
    state = 3

    if (distanceRight < GESTURE_DIST):

        while distanceLeft > GESTURE_DIST:
            distanceLeft, distanceRight = readSensors()
        while distanceLeft < GESTURE_DIST:
            distanceLeft, distanceRight = readSensors()
        state = 2
        

    elif (distanceLeft < GESTURE_DIST):
        while distanceRight > GESTURE_DIST:
            distanceLeft, distanceRight = readSensors()
        while distanceRight < GESTURE_DIST:
            distanceLeft, distanceRight = readSensors()
        state = 1
        
    control_player(state)





