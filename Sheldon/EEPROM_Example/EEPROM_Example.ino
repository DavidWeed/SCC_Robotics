/* EEPROM Test Program
David Weed 12/4/17

This program demonstrates the reading a writing of float data to the interal EEPROM.
Tested using an Arduino\Genuino Uno with nothing connected.
*/

#include <EEPROM.h>

//Prototypes
template<class T>
void eepromWrite(T data);
void eepromReadAll();
void eepromClear();

//stores the next available address
//starts at 4 because it takes up 0,1,2,3
int nextAddress = 4;

//whether you want to write data or read it we could move this to a phyisical switch to avoid code edits
bool read = true;

void setup()
{
	Serial.begin(9600);

	if (read)
	{
		EEPROM.get(0, nextAddress);
		eepromReadAll();
	}
	else
	{
		EEPROM.put(0, nextAddress);
		EEPROM.get(0, nextAddress);
		eepromWrite(3.141f);
		eepromWrite(2.718f);
		eepromWrite(1.234f);
		eepromReadAll();
	}
}


void loop()
{
	//nothing to loop
}


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