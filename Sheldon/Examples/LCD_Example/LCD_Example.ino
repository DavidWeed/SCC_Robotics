#include <LiquidCrystal.h>

#define RS 2
#define EN 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup() 
{
  lcd.begin(16, 2);
  lcd.print("Hello, World!");
}

void loop() 
{  
  lcd.setCursor(0, 1);
  lcd.print(millis()/1000);
}

