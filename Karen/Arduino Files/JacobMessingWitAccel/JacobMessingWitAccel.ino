#include <LSM9DS1_Registers.h>
#include <LSM9DS1_Types.h>
#include <SparkFunLSM9DS1.h>
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include <EEPROM.h>

LSM9DS1 imu;

#define LSM9DS1_M 0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW
#define PRINT_CALCULATED
#define PRINT_SPEED 250 // 250 ms between prints
static unsigned long lastPrint = 0; // Keep track of print time
#define DECLINATION -8.58 // Declination (degrees) in Boulder,

//************************OUR STUFF BEINGS**************************************

#include <Servo.h>
#include <MINDSi.h>

#define PINGL  11
#define PINGR  9
#define PINGC  10

#define MOTORPIN  4
#define FRONTSTEERPIN  5
#define BACKSTEERPIN  6
#define CAMERASTEERPIN 8

#define GPOUTCAM 1 //info to camera to double check if bucket
#define GPOUTGPS 7 //send info to check gps and take picture when at bucket
#define GPIN 2     //info from pi confirming that it is indeed a bucket, if no info is received within a certain timeout then we continue on without moving towards a bucket
#define INTERRUPTPIN 3

#define STOPDISTR 17000
#define STOPDISTL 17000

#define COMMANDNUM 52


Servo frontSteer;
Servo backSteer;
Servo drive;
Servo camera;



int currAddress = 0;
void setup() 
{
  //** SPARKFUN 9DOF STUFF *************************************************
  Serial.begin(115200);
  
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  if (!imu.begin())
  {
    Serial.println("Failed to communicate with LSM9DS1.");
    Serial.println("Double-check wiring.");
    Serial.println("Default settings in this sketch will " \
                  "work for an out of the box LSM9DS1 " \
                  "Breakout, but may need to be modified " \
                  "if the board jumpers are.");
    while (1)
      ;
  }
 //*************** OUR STUFF BEGINS ONCE MORE**********************




 
  //attach pins
 frontSteer.attach(FRONTSTEERPIN);
  backSteer.attach(BACKSTEERPIN);
  drive.attach(MOTORPIN);
  frontSteer.write(90);
  backSteer.write(90);  


int randomTime = millis();


 while( millis() - randomTime < 1000)
 { 
  drive.write(66);
  
 }
}

void loop()
{
 
  
}

void printAccel()
{  
  // Now we can use the ax, ay, and az variables as we please.
  // Either print them as raw ADC values, or calculated in g's.
  Serial.print("A: ");
#ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcAccel helper function to convert a raw ADC value to
  // g's. Give the function the value that you want to convert.
  Serial.print(imu.calcAccel(imu.ax), 2);
  Serial.print(", ");
  Serial.print(imu.calcAccel(imu.ay), 2);
  Serial.print(", ");
  Serial.print(imu.calcAccel(imu.az), 2);
  Serial.println(" g");
#elif defined PRINT_RAW 
  Serial.print(imu.ax);
  Serial.print(", ");
  Serial.print(imu.ay);
  Serial.print(", ");
  Serial.println(imu.az);
#endif

}
void takeAccel()
{
 float GravityInFt = 32.17;
  if ( imu.accelAvailable() )
  {    imu.readAccel();
    // IVE DECIDED WERE GONNA USE X AXIS BC I SAID SO 
  } 
  float currAccel = imu.calcAccel(imu.ax);
  currAccel *= GravityInFt;

  EEPROM.write(currAddress, currAccel);
  currAddress += 2;
  
}



