#include "Declarations.h"

void setup()
{
	Serial.begin(115200);
	ss.begin(57600);

  /**Startup Code **/
	attachInterrupt(INTERRUPTPIN, fullStop, CHANGE);
	pinMode(CAMCHECKOUT, OUTPUT);
	pinMode(CAMCHECKIN, INPUT);
	frontSteer.attach(FRONTSTEERPIN);
	backSteer.attach(BACKSTEERPIN);
	drive.attach(MOTORPIN);
	camera.attach(CAMERASTEERPIN);
	frontSteer.write(90);
	backSteer.write(90);
	setupAccel();
	calculateSoftGeo(N, E, S, W);

	pinMode(8, OUTPUT);

	//For the light to turn on
	pinMode(7, OUTPUT);

	//Test code
	//drive.write(50);
	//delay(3000);
	uTurnRight(turnAngle);
	frontSteer.write(90);
	backSteer.write(90);
	delay(3000);
	drive.write(0); 
}


void loop()
{
	/*if (getPing(PINGC) <= STOPDISTC)
	{
		goAround();
	}

	getKarenPosition();

	Serial.print(karen.x, 10);
	Serial.print(' ');
	Serial.print(karen.y, 10);
	Serial.println();

	if (checkHardGeo())
	{
		//fullStop();
		digitalWrite(8, LOW);
	}
	else
	{
		digitalWrite(8, HIGH);
	}

	if (checkSoftGeo())
	{
		initiateTurn();
	}

	if (getPing(PINGL) <= STOPDISTL)
	{
		cameraCheck(LEFT);
		accelerate(TOPVELOCITY);
	}
	
	if (getPing(PINGR) <= STOPDISTR)
	{
		cameraCheck(RIGHT);
		accelerate(TOPVELOCITY);
	}
	
	drive.write(55);*/
}
