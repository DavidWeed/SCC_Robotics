#!/bin/bash

today=`date '+%m_%d__%H_%M_%S'`;

dirname="/media/sdcard/Bucket_$today/"

#echo $dirname;

#sudo mkdir -p $dirname

sudo cp -r /home/pi/BucketFiles/ $dirname
