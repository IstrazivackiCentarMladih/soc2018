# -*- coding: utf-8 -*-
"""
Created on Sat Jul 28 15:48:08 2018

@author: Lovro
"""

# imports for playing sound
import pyaudio
import wave
import pyglet

# imports for filesystem
import os
import sys


def playSong(audioFile):
    print("idem")
    CHUNK = 1024
    
    wf = wave.open(audioFile, 'rb')
    p = pyaudio.PyAudio()
    
    stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                    channels=wf.getnchannels(),
                    rate=wf.getframerate(),
                    output=True)
    
    data = wf.readframes(CHUNK)
    
    while data != b'':

        stream.write(data)
        data = wf.readframes(CHUNK)
        
    stream.stop_stream()
    stream.close()
    
    p.terminate()
    
    # end function / thread / process
    
def play2(audioFile):
    print('idem2')
    # create a player and queue the song
    player = pyglet.media.Player()
    sound = pyglet.media.load(audioFile)
    player.queue(sound) 

    # keep playing for as long as the app is running (or you tell it to stop):
    player.eos_action = pyglet.media.SourceGroup.loop

    player.play()

    

audioFile = sys.argv[1]
play2(audioFile)