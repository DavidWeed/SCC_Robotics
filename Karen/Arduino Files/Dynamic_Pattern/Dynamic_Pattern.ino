#include <Servo.h>
#include <MINDSi.h>
#include "Declarations.h"

Servo frontSteer;
Servo backSteer;
Servo drive;

Point N = {0, 0};
Point E = {0, 0};
Point S = {0, 0};
Point W = {0, 0};

Karen karen = {0, 0};

void setup()
{
  /**Startup Code **/
	attachInterrupt(INTERRUPTPIN, fullStop, CHANGE);
	pinMode(CAMCHECKOUT, OUTPUT);
	pinMode(GPSGETOUT, OUTPUT);
	pinMode(CAMCHECKIN, INPUT);
	frontSteer.attach(FRONTSTEERPIN);
	backSteer.attach(BACKSTEERPIN);
	drive.attach(MOTORPIN);
	frontSteer.write(90);
	backSteer.write(90);
}

void loop()
{
	getKarenPosition();

	if (checkHardGeo())
	{
		fullStop();
	}
	
	if (checkSoftGeo())
	{
		initiateTurn();
	}

	if (getPing(PINGL) <= STOPDISTL)
	{

	}

	if (getPing(PINGR) <= STOPDISTR)
	{

	}

}
