#include "Declarations.h"

void setup()
{
	Serial.begin(115200);
	ss.begin(57600);

  /**Startup Code **/
	//attachInterrupt(INTERRUPTPIN, fullStop, CHANGE);
	pinMode(CAMCHECKOUT, OUTPUT);
	//pinMode(CAMCHECKIN, INPUT);
	frontSteer.attach(FRONTSTEERPIN);
	backSteer.attach(BACKSTEERPIN);
	drive.attach(MOTORPIN);
	camera.attach(CAMERASTEERPIN);
	frontSteer.write(90);
	backSteer.write(90);
	drive.write(90);
	gpsDelay(750);
	calculateSoftGeo(N, E, S, W);

	pinMode(8, OUTPUT);
	/*
	karen.x = -6;
	karen.y = 4;
	*/
	
	drive.write(50);
	turnLeft(turnAngle);
}

void loop()
{

	getKarenPosition();

	drive.write(50);

	if (getPing(PINGC) <= STOPDISTC)
	{
		goAround();
	}
		

	if (checkHardGeo())
	{
		fullStop();
		digitalWrite(8, HIGH);
		Serial.println("Outside\n");
	}
	else
	{
		digitalWrite(8, LOW);
		Serial.println("Inside");
	}

	
	if (checkSoftGeo())
	{
		initiateTurn();
	}
  
  
	if (getPing(PINGL) <= STOPDISTL)
	{
		cameraCheck(LEFT);
	}
	
	if (getPing(PINGR) <= STOPDISTR)
	{
		cameraCheck(RIGHT);
	}
	
}