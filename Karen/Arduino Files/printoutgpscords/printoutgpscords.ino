#include <SoftwareSerial.h>

#include <TinyGPS.h>

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

SoftwareSerial ss(4, 3);
TinyGPS gps; 

void setup()
{
  Serial.begin(115200);
 
  ss.begin(57600);
}

void loop()
{
  long flat, flon;
  
  gps.get_position(&flat, &flon);
  
  Serial.print(flat);
  Serial.print(" ");
  Serial.print(flon);
  Serial.println();
  
  smartdelay(1000);
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

