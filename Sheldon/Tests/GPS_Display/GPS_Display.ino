#include <Servo.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <LiquidCrystal.h>

/** Prototypes **/
uint16_t get_ping(int pin, uint16_t max_micros = 25000);

static void smart_delay(unsigned long ms);

TinyGPS gps;
SoftwareSerial ss(A5, 12);
long lat, lon;

#define RS 2
#define EN 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);


void setup()
{
  lcd.begin(16, 2);  
  ss.begin(57600);  

  smart_delay(1000);
}


void loop()
{
  smart_delay(100);
   
  gps.get_position(&lat, &lon);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(lat);
  lcd.setCursor(1,0);
  lcd.print(lon);
}


static void smart_delay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

