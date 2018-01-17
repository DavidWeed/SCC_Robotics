/* GPS Example
David Weed 12/17/2017

Prints GPS data to the Serial monitor.

Tested using the LS20031 GPS from Sparkfun.
Connected GPS Pin Tx to Arduino Pin Rx as well as 3v3 and GND to pin 4 and 1.
*/

#define USE_TINYGPS false

#if USE_TINYGPS == true

/*GPS Pin 3 to Ard Pin 4*/

#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial ss(4, 3); //(rx, tx)

static void smartdelay(unsigned long ms);


void setup()
{
	Serial.begin(115200);
	ss.begin(57600);
}

void loop()
{
	float flat, flon;

	gps.f_get_position(&flat, &flon);
	Serial.print("Lat:  ");
	Serial.print(flat, 9);
	Serial.print(", Lon:  ");
	Serial.println(flon, 9);

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

#else

/*GPS Pin 3 to Ard Pin 0*/

void setup()
{
	Serial.begin(57600);
}

void loop()
{
	if (Serial.available())
	{
		Serial.write(Serial.read());
	}
}

#endif