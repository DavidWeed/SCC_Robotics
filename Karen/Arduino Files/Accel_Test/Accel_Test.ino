#include <Servo.h>

Servo servo;

const int servoPin = 10;
const int xpin = A1;
const int ypin = A2;
const int zpin = A3;

const int groundpin = A0;            
const int powerpin = A4; 

void setup() 
{
	Serial.begin(9600);

	servo.attach(servoPin);

	pinMode(groundpin, OUTPUT);
	pinMode(powerpin, OUTPUT);
	digitalWrite(groundpin, LOW);
	digitalWrite(powerpin, HIGH);

}

void loop() 
{    
	Serial.println(analogRead(xpin));  	
	delay(100);
	
	int tilt = analogRead(xpin);

	//if (tilt <= 350 && tilt >= 330)
	//{
	//	servo.write(90);
	//}
	//else if (tilt <= 310  && tilt >= 290)
	//{
	//	servo.write(10);
	//}
	//else if (tilt <= 370 && tilt >= 390 )
	//{
	//	servo.write(160);
	//}

	servo.write(map(tilt, 200, 500, 0, 179));
}
