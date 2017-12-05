/* EEPROM Test Program
David Weed 12/4/17

This program demonstrates the reading a writing of float data to the interal EEPROM.
Tested using an Arduino\Genuino Uno with nothing connected.
*/

#include <EEPROM.h>



void setup()
{
	
}


void loop()
{

}


void eepromClear()
{
	for (int i = 0; i < EEPROM.length(); i++)
	{
		EEPROM.write(i, 0);
	}
}