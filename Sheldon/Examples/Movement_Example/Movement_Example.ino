/* Movement Example
David Weed 1/16/2018

Sheldon should be setup as follows

   Pin   <->   Arduino
-----------------------
  Drive           3
Front Servo       5
Rear Servo        6

This example moves Sheldon forward left and then right then stops.
*/

#include <Servo.h>

Servo driveServo;
Servo frontServo;
Servo backServo;

void setup()
{
	driveServo.attach(3);
	frontServo.attach(5);
	backServo.attach(6);

	driveServo.write(90);
	frontServo.write(90);
	backServo.write(90);

	driveServo.write(70);
	delay(500);
	frontServo.write(45);
	backServo.write(45);
	delay(500);
	frontServo.write(135);
	backServo.write(135);
	delay(500);

	driveServo.write(90);
	frontServo.write(90);
	backServo.write(90);
}

void loop()
{

}
