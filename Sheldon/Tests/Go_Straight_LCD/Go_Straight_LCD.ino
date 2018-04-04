#include <Servo.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <LiquidCrystal.h>

/** Prototypes **/
//uint16_t get_ping(int pin, uint16_t max_micros = 25000);
uint16_t get_ping(int pin);

static void smart_delay(unsigned long ms);


/** Declarations **/
Servo drive;
Servo front_steer;
Servo back_steer;

#define BRAKE    0
#define NEUTRAL 90
#define DRIVE   70

#define left_ping   A2
#define front_ping  A1
#define right_ping  A0

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

int survivor_count = 0;

/** Setup Function **/
void setup()
{
  lcd.begin(16, 2);  
  ss.begin(57600);
  
  drive.attach(9);
  front_steer.attach(10);
  back_steer.attach(11);

  drive.write(90);
  front_steer.write(90);
  back_steer.write(90);

  smart_delay(1000);
}


/** Loop Function **/
void loop()
{
  smart_delay(200);
   
  gps.get_position(&lat, &lon);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(lat);
  lcd.setCursor(1,0);
  lcd.print(lon);
  lcd.setCursor(13, 0);
  lcd.print(survivor_count);
  
  if(get_inches(front_ping) <= 36) //stop 3 feet before crash
  {
    drive.write(0);
    smart_delay(20);
    drive.write(90);    
  }
  else
  {
    drive.write(70);
  }
  
  //drive.write((get_inches(front_ping) <= 36)? BRAKE : DRIVE);
  
  if(get_inches(left_ping) <= 120 || get_inches(right_ping) <= 120)
  {
    survivor_count++;
  }
}


/** Custom Functions Definitions **/

int get_inches(int pin)
{
  return get_ping(pin) / 74 / 2;
}


//uint16_t get_ping(int pin, uint16_t max_micros = 25000)
uint16_t get_ping(int pin)
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
    /*
    if(inputpulse == 0) inputpulse = max_micros;
    delayMicroseconds(200);
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    return inputpulse;
    */
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

