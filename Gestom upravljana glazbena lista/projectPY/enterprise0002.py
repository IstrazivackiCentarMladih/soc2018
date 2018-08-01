# -*- coding: utf-8 -*-
"""
Created on Thu Jul 26 18:52:13 2018

@author: Lovro
"""

import time

# imports for filesystem
import os

# import for multiprocessing
import subprocess

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

gesture = 3
pro = play(index)

#while True:
    
    #gesture = int(input("Where?")) #state
def control_player(gesture):

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
    


