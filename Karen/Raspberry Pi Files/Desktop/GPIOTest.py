import RPi.GPIO as GPIO
import subprocess

myled1 = 2
myled2 = 3
myled3 = 4

button1 = 17
button2 = 27

GPIO.setwarnings(False)

GPIO.setmode(GPIO.BCM)

GPIO.setup(myled1, GPIO.OUT)
GPIO.setup(myled2, GPIO.OUT)
GPIO.setup(myled3, GPIO.OUT)

GPIO.setup(button1, GPIO.IN)
GPIO.setup(button2, GPIO.IN)


try:
    while 1:
        if GPIO.input(button1):
            #GPIO.output(myled1, GPIO.HIGH)
            subprocess.call(["python", "/home/pi/Desktop/Bucket Recognition/cvTest.py"]) 
        #else:
            #GPIO.output(myled1, GPIO.LOW)
            
except KeyboardInterrupt:
        GPIO.cleanup()


