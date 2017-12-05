#David Weed
#Computer Vision 
#Detect an orange Home Depot bucket in the screen

#run on trigger from GPIO

import cv2
import numpy as np
import RPi.GPIO as GPIO
from picamera import PiCamera
from time import sleep

imageName = '/home/pi/BucketFiles/image.jpg'
keypointImageName = '/home/pi/BucketFiles/keypoint.jpg'

camera = PiCamera()

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(4, GPIO.OUT)

#set the parameters of takeing the intial image

camera.capture(imageName)

#set the parameters of the detector that will find the buckets

params = cv2.SimpleBlobDetector_Params()

params.filterByArea = True
params.minArea = 5000
params.maxArea = 10000000000

params.filterByCircularity = True
params.maxCircularity = 1
params.minCircularity = 0.1
params.filterByConvexity = True
params.maxConvexity = 1
params.minConvexity = 0.01
params.filterByInertia = True
params.maxInertiaRatio = 1
params.minInertiaRatio = 0.001


image = cv2.imread(imageName)
hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

# give HSV values Ranges H 0-180 S,V 0-255

#searches for reds and oranges at lower end
lower_orange = np.array([0, 130, 100])
upper_orange = np.array([10, 255, 255])

#reds at higher end 
lower_orange2 = np.array([176, 130, 100])
upper_orange2 = np.array([180, 255, 200])

detector = cv2.SimpleBlobDetector_create(params)        

mask1 = cv2.inRange(hsv, lower_orange, upper_orange)
mask2 = cv2.inRange(hsv, lower_orange2, upper_orange2)

mask = cv2.bitwise_or(mask1, mask2)

inv = cv2.bitwise_not(mask)

keypoints = detector.detect(inv)

keypoint_im = cv2.drawKeypoints(inv, keypoints, np.array([]), (0,255,0), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

cv2.imwrite(keypointImageName, keypoint_im)

if keypoints:
        print "bucket found"
        print keypoints        
        GPIO.output(4, GPIO.HIGH)
else:
        print "no bucket found"
        GPIO.output(4, GPIO.LOW)

sleep(2)

camera.stop_preview()
cv2.destroyAllWindows()
GPIO.cleanup()


del keypoints
del detector
del camera
del cv2
del GPIO
