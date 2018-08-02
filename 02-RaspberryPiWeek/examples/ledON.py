#/usr/bin/env python3
LED_PIN = 40

import gpiozero
import time

led_40 = gpiozero.LED(LED_PIN)

while True:
    led_40.on()
    time.sleep(1.0)

