
import RPi.GPIO as GPIO

def set(pin, val):
    print('setting %d to %d'%(pin, val))
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, val)

def off(pin):
    set(pin, 0)

GPIO.setmode(GPIO.BOARD)

pins = [21, 23, 22, 24, 31, 32]
for pin in pins:
    off(pin)
