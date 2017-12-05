
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>

LSM9DS1 imu;

void setup() 
{
  Serial.begin(115200);
    
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = 0x1E;
  imu.settings.device.agAddress = 0x6B;

  if (!imu.begin())
  {
    Serial.println("Failed to communicate with LSM9DS1.");
    while (1)
      ;
  }
}

void loop() 
{
 if ( imu.magAvailable() )
  {
    //update mx, my, and mz variables
    imu.readMag();
  }

//  Serial.print(imu.calcMag(imu.mx), 2);
//  Serial.print(", ");
//  Serial.print(imu.mx);
//  Serial.print(", ");
//  Serial.print(imu.calcMag(imu.my), 2);
//  Serial.print(", ");
//  Serial.print(imu.my);
//  Serial.print(", ");
//  Serial.print(imu.calcMag(imu.mz), 2);
//  Serial.print(", ");
//  Serial.print(imu.mz);  
//  Serial.println(" gauss");
  delay(200);


  float heading;
  if (-imu.my == 0)
    heading = (-imu.mx < 0) ? PI : 0;
  else
    heading = atan2(-imu.mx, -imu.my);
    
  heading -= 11.91 * PI / 180;
 
  if (heading > PI) heading -= TWO_PI;
  else if (heading < -PI) heading += TWO_PI;
  else if (heading < 0) heading += TWO_PI;

  heading *= 180.0 / PI;
  
  Serial.print("Heading: "); Serial.println(heading);
}
