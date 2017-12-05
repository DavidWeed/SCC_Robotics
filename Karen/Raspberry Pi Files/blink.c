#include <wiringPi.h>

int main(void)
{
    wiringPiSetup();
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(7, OUTPUT);

    while(1)
    {
        digitalWrite(8, HIGH);
	delay(250);
	digitalWrite(9, HIGH);
	delay(250);
	digitalWrite(7, HIGH);
	delay(250);
	digitalWrite(7, LOW);
	delay(250);
	digitalWrite(9, LOW);
	delay(250);
	digitalWrite(8, LOW);
	delay(250);
    }

    return 0;
}
