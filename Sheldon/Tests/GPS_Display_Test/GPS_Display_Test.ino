/* GPS Geofence Test
David Weed & Brian Choi 

Following standard Sheldon pin guide

Displays the GPS location on the LCD and serial monitor,
also displays an average of four readings on the same out puts 
*/

#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <LiquidCrystal.h>

/**LCD Setup**/
//define makes the compiler replace the first argument with the second argument (ex: replaces RS with 2)
#define RS 2 
#define EN 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

/**GPS pin Definition**/
#define GPS_TX A5

TinyGPS gps;
//Here 12 is unused because we don't need to send data to the GPS
SoftwareSerial ss(GPS_TX, 12); //Arduino's (Receive_Pin, Transmit_Pin)
long lat, lon;

long avg_lat_total, avg_lon_total;
long avg_lat[4] = {0,0,0,0}; 
long avg_lon[4] = {0,0,0,0};


void setup()
{
  Serial.begin(9600);
  ss.begin(57600);
  lcd.begin(16, 2);
}


void loop()
{  
  //get the GPS position
  gps.get_position(&lat, &lon);

  //update LCD
  lcd.setCursor(1,0);
  lcd.print(lat);
  lcd.setCursor(0,1);
  lcd.print(lon);

  //send to serial monitor
  Serial.print("Orig: ");
  Serial.print(lat);
  Serial.print(", ");
  Serial.println(lon);

  //get the next four readings over two seconds
  for(int i = 0; i < 4; i++)
  {
    smartdelay(500);
    gps.get_position(&avg_lat[i], &avg_lon[i]);
  }

  //average those readings
  avg_lat_total = average_arr(avg_lat);
  avg_lon_total = average_arr(avg_lon);
  
  //send to serial monitor
  Serial.print("Avg:  ");
  Serial.print(avg_lat_total);
  Serial.print(", ");
  Serial.println(avg_lon_total);
  Serial.println("");

  //add averages to lcd
  lcd.setCursor(12, 0);
  lcd.print(avg_lat_total % 1000);
  lcd.setCursor(11, 1);
  lcd.print(avg_lon_total % 1000);
  
  //get new data from the GPS sensor
  smartdelay(500);
}


//average 4 values in an array
static long average_arr(long arr[4])
{
  return (((arr[0]+arr[1])/2)+((arr[2]+arr[3])/2))/2;
}


//Update the data in the gps object with data from the sensor
static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}


