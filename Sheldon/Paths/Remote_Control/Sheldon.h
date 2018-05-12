/* Sheldon.h  
* Contains all of the declarations and definitions for functions and variables
*/

//********** Movement **********//

#include <Servo.h>

Servo drive;
Servo front_steer;

void driveSetup()
{
  drive.attach(9);
  front_steer.attach(10);
}

//********** GPS **********//

#include <TinyGPS.h>

TinyGPS gps;
long lat, lon;

#if defined(__AVR_ATmega32U4__) //if we are using the Leonardo

  void gpsSetup()
  {
    Serial1.begin(57600);
  }
  
	static void gpsDelay(unsigned long ms)
	{
		unsigned long start = millis();
		do
		{
			while (Serial1.available())
				gps.encode(Serial1.read());
		} while (millis() - start < ms);
    gps.get_position(&lat, &lon);
	}

#else
	
	void gpsSetup()
  {
    Serial.begin(57600);
  }
  
	static void gpsDelay(unsigned long ms)
	{
		unsigned long start = millis();
		do
		{
			while (Serial.available())
				gps.encode(Serial.read());
		} while (millis() - start < ms);
    gps.get_position(&lat, &lon);
	}

#endif


//********** EEPROM **********//

#include <EEPROM.h>

int nextAddress = 4;

template<class T>
void eepromWrite(T data)
{
  EEPROM.put(nextAddress, data);
  nextAddress += sizeof(data);
  EEPROM.put(0, nextAddress);
}

void eepromReadAll()
{
	float tempData;
	for (int i = 4; i < nextAddress; i += 4)
	{
		EEPROM.get(i, tempData);
		Serial.println(double(tempData), 3);
	}
}

void eepromClear()
{
	for (int i = 0; i < EEPROM.length(); i++)
	{
		EEPROM.write(i, 0);
	}
}


//********** Geofence **********//

struct Point
{
  long x;
  long y;
};

Point N = {-117808968, 33780725}, 
    E = {-117808937, 33780529}, 
    S = {-117809090, 33780495}, 
    W = {-117809131, 33780656};
  
float calculate_y(Point one, Point two)
{   
  float slope = (float)(two.y - one.y) / (float)(two.x - one.x);  
  return slope * (lon - one.x) + one.y;
}

bool is_outside_geofence()
{
  return lat < calculate_y(S, E) ||
       lat < calculate_y(W, S) ||
       lat > calculate_y(E, N) ||
       lat > calculate_y(N, W);
}

//********** LCD **********//

#include <LiquidCrystal.h>

#define RS 2
#define EN 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7); 

void lcdSetup()
{
  lcd.begin(16, 2);
}

void lcd_show_lat_lon()
{
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(lat);
	lcd.setCursor(0,1);
	lcd.print(lon);
}
