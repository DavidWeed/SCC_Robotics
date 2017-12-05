from picamera import PiCamera
from time import sleep

camera = PiCamera()

#camera.capture('image.jpg')
camera.start_preview()

sleep(15)

camera.stop_preview()
