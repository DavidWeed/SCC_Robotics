  /* Basic Geofence Test
   *  David Weed & Brian Choi 
   *  
   *  Follows Sheldon Pin Guide
   *  
   *  Determines if Arduino is within the Geofence
   * 
   * 3/15/18: Fixed Serial interrupt issue
  */
  
#include <TinyGPS.h>
#include <LiquidCrystal.h>
#include <Servo.h>
  
#define RS 2
#define EN 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

#define GPS_TX 8
TinyGPS gps;
long lat, lon;
static void gps_delay(unsigned long ms);

struct Point
{
  long x;
  long y;
};

Point N = {-117808968, 33780725}, 
      E = {-117808937, 33780529}, 
      S = {-117809090, 33780495}, 
      W = {-117809131, 33780656};

Servo drive;
Servo front_steer;
Servo back_steer;

void setup() 
{
  lcd.begin(16,2);
  //Serial.begin(9600);
  Serial1.begin(57600);
  //pinMode(13, OUTPUT);
  
  drive.attach(9);
  front_steer.attach(10);
  
  drive.write(90); //neutral
  front_steer.write(90); //straighten wheels
  
  //drive.write(70);
  gps_delay(500);
  gps.get_position(&lat, &lon);
  gps_delay(500);
  gps.get_position(&lat, &lon);
  gps_delay(500);
  gps.get_position(&lat, &lon);
}


void loop() 
{  
  gps_delay(250);  
  
  //update current position
  gps.get_position(&lat, &lon);
  
  //Serial.print(lat);
  //Serial.print(",");
  //Serial.println(lon);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(lat);
  lcd.setCursor(0,1);
  lcd.print(lon);
  
  //digitalWrite(13, !is_outside_geofence());
  
  if(is_outside_geofence())
  {
    while(true)
    {
      drive.write(0);
      gps_delay(2000);
      if(!is_outside_geofence())
        break;
    }
  }
  else
  {    
    drive.write(70);
  }  
}


static void gps_delay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}


bool is_outside_geofence()
{
  return lat < calculate_y(S, E) ||
         lat < calculate_y(W, S) ||
         lat > calculate_y(E, N) ||
         lat > calculate_y(N, W);
}


float calculate_y(Point one, Point two)
{ 
  //Serial.println(two.y - one.y);
  //Serial.println(two.x - one.x);
  //Serial.println((float)(two.y - one.y) / (float)(two.x - one.x));  
  float slope = (float)(two.y - one.y) / (float)(two.x - one.x);
  //Serial.println(slope);
  //Serial.println();
  return slope * (lon - one.x) + one.y;
}


