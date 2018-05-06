#include "Sheldon.h"

void setup() 
{ 
  move_setup();
  serial_setup();
  lcd_setup();
  
  drive.write(90);
  front_steer.write(90);
  gps_delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:

}
