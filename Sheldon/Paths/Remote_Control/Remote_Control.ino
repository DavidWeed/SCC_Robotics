#include "Sheldon.h"

int prevSig;

void setup() 
{
  lcdSetup();
  gpsSetup();
  lcd.clear();
  gpsDelay(500);
  lcd_show_lat_lon();
  pinMode(A0, INPUT);  
}
 
void loop() 
{
  prevSig = pulseIn(A0, HIGH);
  gpsDelay(200);
  lcd_show_lat_lon();
  int diff = prevSig - pulseIn(A0, HIGH);
  bool change = ((diff > 0)? diff : -diff) > 300;
  if(change)
  {//insert save GPS code here//
    eepromWrite(lat);
    eepromWrite(lon); 
  }
}

