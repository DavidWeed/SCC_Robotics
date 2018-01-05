Karen was the attempt from 2016

The main system design was as follows:

-Karen would follow a path that was made of commands {s, l ,r}</br>
-while moving she would use Ping sensors to check for objects on her sides</br>
-if an object was detected a signal was sent to the Raspberry Pi to double check</br>
-The RasPi used OpenCV in a python script to determien if there was an orange bucket there</br>
-if there was then the GPS data was stored using GPSD in a bash script</br>
-control was then returned to the Arduino and she continued moving</br>

Overal this system was designed to account for a lot of edge cases and was made from a point of trying to retrive every bucket on the field. This meant that it grew to be too large and ended up not working properly on competition day. </br>

The files included here are sorted by the system they were used on:</br>

Arduino Files</br>
--Contains many test files and different projects by different people.</br>
--Main components:</br>
  -Ping Sensors</br>
  -Motor Control</br>
  -Triggering the Raspi</br></br>

Raspberry Pi Files</br>
--Contains example files as well as the scripts used</br>
--Main Components:</br>
  -GPS data retrieval and parsing</br>
  -Computer Vision bucket detection</br>
  
