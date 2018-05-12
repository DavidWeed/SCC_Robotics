  /* Pinball Path 5/11/2018
   *  David Weed & Brian Choi 
   *  
   * Sheldon calculates a geofence and uses it to bounce around the field.  
  */
  
#include <TinyGPS.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <math.h>
#include <EEPROM.h>

int nextAddress = 4;
  
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
long prevLat, prevLon;

float angleS;
float *linePassed;

int frontPing = A1;
int leftPing = A2;
int rightPing = A0;

struct Point
{
  long x;
  long y;
};

static void gps_delay(unsigned long ms);
float calculate_y(float slope, Point one, Point two);
float sheldonAngle(long y2, long y1, long x2, long x1);
bool turnRight(float sheldonAngle);

Point N = {-117808968, 33780725}, 
      E = {-117808937, 33780529}, 
      S = {-117809090, 33780495}, 
      W = {-117809131, 33780656};

float northEast = (float)(N.y - E.y) / (float)(N.x - E.x); //Slopes of the geofence edges
float northWest = (float)(N.y - W.y) / (float)(N.x - W.x);
float southEast = (float)(S.y - E.y) / (float)(S.x - E.x);
float southWest = (float)(S.y - W.y) / (float)(S.x - W.x);

/*
float angleNE = atan(abs(northEast)); //These are the angles the geofences lines make with the x-axis
float angleNW = atan(abs(northWest)); //not sure why we're not using abs()
float angleSE = atan(abs(southEast));
float angleSW = atan(abs(southWest));
*/

float angleNE = atan(northEast);                       //These are the angles the geofences lines make with the x-axis
float angleNW = atan(northWest);
float angleSE = atan(southEast);
float angleSW = atan(southWest);

float radius = 10;                                    //Every quarter circle turn has a radius of 10 ft
float speed = 3;                                      //Sheldon moves at a speed of 1.5 ft/s (maybe??)
//float timeToTurn = ((2*PI*radius)/4)/speed * 1000;    //Multiplying by 1000 converts to milliseconds
float timeToTurn = 4800;

Servo drive;
Servo front_steer;

void setup() 
{  
  lcd.begin(16,2);
  Serial1.begin(57600);
  
  drive.attach(9);
  front_steer.attach(10);
  
  drive.write(90);         //neutral
  front_steer.write(90);   //straighten wheels
  
  gps_delay(500);                //make sure the gps is updated
  gps.get_position(&lat, &lon);
  gps_delay(500);
  gps.get_position(&lat, &lon);
  gps_delay(500);
  gps.get_position(&lat, &lon);

  if(digitalRead(A5) == HIGH)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Reading EEPROM");
    Serial.begin(9600);  
    while(!Serial)
    {;} 
    long tempData;
    //EEPROM.get(0, nextAddress);
    for (int i = 4; i < EEPROM.length() - 4; i += 4)
    {
      EEPROM.get(i, tempData);
      Serial.print(tempData);
      i += 4;
      Serial.print(", ");
      EEPROM.get(i, tempData);
      Serial.println(tempData);
    }
    while(1){delay(100);};
  }

  drive.write(70);
}


void loop() 
{  
  gps_delay(250);  
  
  prevLat = lat;
  prevLon = lon;

  gps.get_position(&lat, &lon);                         //update current position
  angleS = sheldonAngle(prevLat, lat, prevLon, lon);    //update the angle Sheldon's line of travel makes with the x-axis
  
  lcd.clear();
  lcd.setCursor(15,0);
  lcd.print("S");
  lcd.setCursor(0,0);
  lcd.print(lat);
  lcd.setCursor(0,1);
  lcd.print(lon);
  
  if(getInches(frontPing) < 120)
  {
    drive.write(0);
    eepromWrite(lat);
    eepromWrite(lon);
    delay(1000);                                        //Sheldon visibly acknowledges there is something in front of him
    front_steer.write(83);     //Sheldon turns left 
    gps_delay(timeToTurn);
    front_steer.write(90);
    drive.write(90); //disengage brake
  } 
  else if(getInches(leftPing) < 120)
  {
    drive.write(0);
    eepromWrite(lat);
    eepromWrite(lon);
    delay(1000);                                        //Sheldon visibly acknowledges there is something to the left of him
    front_steer.write(97);     //Sheldon turns right 
    gps_delay(timeToTurn);
    front_steer.write(90);
    drive.write(90); //disengage brake
  }
  else if(getInches(rightPing) < 120)
  {
    drive.write(0);
    eepromWrite(lat);
    eepromWrite(lon);
    delay(1000);                                        //Sheldon visibly acknowledges there is something to the right of him
    front_steer.write(83);     //Sheldon turns left 
    gps_delay(timeToTurn);
    front_steer.write(90);
    drive.write(90); //disengage brake
  }  
  
  if(is_outside_geofence())
  {
    if(turnRight(angleS))
    {
      lcd.setCursor(15,0);
      lcd.print("R");
      drive.write(70);
      front_steer.write(120);
      gps_delay(timeToTurn);
      front_steer.write(90);
      gps_delay(1000);
    }
    else    
    {
      lcd.setCursor(15,0);
      lcd.print("L");
      drive.write(70);
      front_steer.write(60);
      gps_delay(timeToTurn);
      front_steer.write(90);
      gps_delay(1000);
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
  if(lat < calculate_y(southEast, S, E))
  {
    linePassed = &angleSE;
  }
  else if(lat < calculate_y(southWest, W, S))
  {
    linePassed = &angleSW;
  }
  else if(lat > calculate_y(northEast, E, N))
  {
    linePassed = &angleNE;
  }
  else if(lat > calculate_y(northWest, N, W))
  {
    linePassed = &angleNW;
  }
  else
  {
    return false;
  }
  
  return true;
}


float calculate_y(float slope, Point one, Point two)
{ 
  return slope * (lon - one.x) + one.y;
}

float sheldonAngle(long y2, long y1, long x2, long x1)
{
  return atan(abs((float)(y2 - y1)/(float)(x2 - x1)));
}

bool turnRight(float angleS)
{
  float angleDiff = angleS - *linePassed;
  return((angleDiff < 0 && *linePassed == angleNE) ||
         (angleDiff > 0 && *linePassed == angleNW) ||
         (angleDiff < 0 && *linePassed == angleSW) ||
         (angleDiff > 0 && *linePassed == angleSE) );
}

int getPing(int pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(5);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);
  delayMicroseconds(5);
  pinMode(pin, INPUT);
  return pulseIn(pin, HIGH);  
}

int getInches(int pin)
{
  return getPing(pin) / 74 / 2;
}

void eepromWrite(long data)
{
  EEPROM.put(nextAddress, data);
  nextAddress += sizeof(data);
  EEPROM.put(0, nextAddress);
}

