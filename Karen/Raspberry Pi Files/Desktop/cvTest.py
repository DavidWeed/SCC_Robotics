#David Weed
#Computer Vision 
#Detect an orange Home Depot bucket in the screen

#run on trigger from GPIO

import cv2
import numpy as np
#import RPi.GPIO as GPIO
from picamera import PiCamera

imageName = 'image.jpg'

cap = cv2.VideoCapture(0) 
camera = PiCamera()
camera.capture(imageName)

params = cv2.SimpleBlobDetector_Params()

params.filterByArea = True
params.minArea = 200
#params.maxArea = 100000

params.minThreshold = 0
params.maxThreshold = 255

params.filterByCircularity = True
params.minCircularity = 0.1
params.filterByConvexity = True
params.minConvexity = 0.1
params.filterByInertia = True
params.minInertiaRatio = 0.01

while(1):
        image = cv2.imread(imageName)
        hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

        # give HSV values Ranges H 0-180 S,V 0-255
        lower_orange = np.array([0, 100, 100])
        upper_orange = np.array([15, 255, 255])

        detector = cv2.SimpleBlobDetector_create(params)        
        #detector = cv2.SimpleBlobDetector_create()
                        
        mask = cv2.inRange(hsv, lower_orange, upper_orange)
        res = cv2.bitwise_and(image, image, mask= mask)

        inv = cv2.bitwise_not(mask)
        
        keypoints = detector.detect(inv)

        keypoint_im = cv2.drawKeypoints(inv, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
        
        #cv2.imshow('frame',image)
        #cv2.imshow('mask',mask)
        #cv2.imshow('res',res)
        #cv2.imshow('Keypoints', keypoint_im)
        cv2.imwrite('Keypoints.jpg', keypoint_im)
        
        k = cv2.waitKey(5) & 0xFF
        
        #wait for ESC key
        #if k == 27:
        #        break
        break

if keypoints:
        print "bucket found"
        print keypoints
else:
        print "no bucket found"

#output to GPIO or stdout
        
#using pin 3 on the board
#second row down 2nd pin in
#GPIO.setmode(GPIO.BCM)
#GPIO.setup(2, GPIO.OUT)
#GPIO.output(2, GPIO.HIGH)

#import sys
#sys.stdout.write("Bucket Found" + '\n')

camera.stop_preview()
cv2.destroyAllWindows()

del keypoints
del detector
del cap
del camera
del cv2
