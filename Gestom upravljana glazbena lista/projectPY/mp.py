# -*- coding: utf-8 -*-
"""
Created on Thu Jul 26 18:52:13 2018

@author: Lovro
"""

import serial
import time

# imports for filesystem
import os

# import for multiprocessing
import subprocess as sp


pro = sp.Popen(['mpg123', 'play_list/sound2.mp3'],
        stderr=sp.DEVNULL,
        stdin =sp.DEVNULL)

term = input("Terminate? [Y/n] ")

if (not term):
    pro.terminate()
