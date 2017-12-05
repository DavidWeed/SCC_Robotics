#! /bin/sh

#setup gpio pins

gpio -g mode 17 down
gpio -g mode 27 down

while true
do
	if [ "$(gpio -g read 17)" = "1" ]; then
		echo "Run Camera"
		python /home/pi/Karen/CheckBucket.py
		
	else
		echo "pin LOW"
	fi
	
	if [ "$(gpio -g read 27)" = "1"]; then
		echo "Get GPS"
		
	else
		echo "pin LOW"
	fi
	
sleep .5
done
