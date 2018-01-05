Karen was the attempt from 2016

The main system design was as follows:

-Karen would follow a path that was made of commands {s, l ,r}
-while moving she would use Ping sensors to check for objects on her sides
-if an object was detected a signal was sent to the Raspberry Pi to double check
-The RasPi used OpenCV in a python script to determien if there was an orange bucket there
-if there was then the GPS data was stored using GPSD in a bash script
-control was then returned to the Arduino and she continued moving

Overal this system was designed to account for a lot of edge cases and was made from a point of trying to retrive every bucket on the field. This meant that it grew to be too large and ended up not working properly on competition day. 

The files included here are sorted by the system they were used on:

Arduino Files
--Contains many test files and different projects by different people.
--Main components:
  -Ping Sensors
  -Motor Control
  -Triggering the Raspi

Raspberry Pi Files
--Contains example files as well as the scripts used
--Main Components:
  -GPS data retrieval and parsing
  -Computer Vision bucket detection
  
