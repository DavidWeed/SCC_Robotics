/* GPS Geofence Test
David Weed & Brian Choi 

Following standard Sheldon pin guide

Keeps track of the min and max lat and lon values,
averages them to find a middle point
*/

#include <SoftwareSerial.h>
#include <TinyGPS.h>


/**GPS pin Definition**/
#define GPS_TX A5

TinyGPS gps;
//Here 12 is unused because we don't need to send data to the GPS
SoftwareSerial ss(GPS_TX, 12); //Arduino's (rx, tx)
long lat, lon;

long min_lat = 1000000000L;
long max_lat = -1000000000L;
long min_long = 1000000000L;
long max_long = -1000000000L;

void setup()
{
  Serial.begin(9600);
  ss.begin(57600);
}

void loop()
{
  gps.get_position(&lat, &lon);

  //check if a point is the new max or min
  if(lat < min_lat)
  {
    min_lat = lat;
  }
  else if(lat > max_lat)
  {
    max_lat = lat;
  }
  
  if(lon < min_long)
  {
    min_long = lon;
  }
  else if(lon > max_long)
  {
    max_long = lon;
  }

  //display average of min/max to serial monitor
  Serial.print((min_lat + max_lat)/2);
  Serial.print(", ");
  Serial.print((min_long + max_long)/2);

  //show how many seconds its been
  Serial.print("  millis = ");
  Serial.println(millis()/1000);
   
  smartdelay(500);
}


static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}


