# include "margoulinade.h"

//EEPROM DATA
// SLOT 0: MEM CLEARED ? 1 -> yes 
// SLOT 1: PASSWORD SET ? 1 -> yes
// SLOT 3: BUZZER DISABLED ? 1 -> yes

void    checkMem()
{
  if(EEPROM.read(0) != 0)
  {
      lcd.print(F("RESET EEPROM"));
      for (int i = 0 ; i < 1023 ; i++)
      {
        EEPROM.write(i, 0);
      }
      lcd.setCursor(0, 1);
      lcd.print(F("OK"));
      wait4button();
  }
}

void    eepromDebug()
{
  int value;
  int address;
  
  while(wait4button)
  {
    value = EEPROM.read(address);
    lcd.print(address);
    lcd.print(F(":"));
    lcd.print(value);
    lcd.print(F("|"));
    lcd.print(EEPROM.length());
    lcd.print(F("."));
    address++;
    delay(500);
    lcd.clear();
  } 
  wait4button();
}

void    checkPassword()
{
  if (EEPROM.read(1) == 1)
  {
    lcd.print(F("checking th passwd"));
    wait4button();
  }
}

void                about()
{   
    lcd.print(F("By guigur&oborotev"));
    lcd.setCursor(0, 1);
    lcd.print(F("2015-2016 china"));
    wait4button();
}

void                buzzer_eeprom()
{

    lcd.print(F("Buzzer"));
    lcd.setCursor(0, 1);
    if (EEPROM.read(3) == 0)
    {
      lcd.print(F("DISABLE"));
    }
    else
    {
         lcd.print(F("ENABLE"));
    }
    wait4button();
    if (EEPROM.read(3) == 0)
    {
      EEPROM.put(3, 1);  
    }
    else
    {
      EEPROM.put(3, 0);
      silentMode = false;
    }
}

void                batStatus()
{   
    delay(250);
    
    while (digitalRead(encButton))
    {
      lcd.setCursor(0, 0);

      if (digitalRead(BatFull) == 0 && digitalRead(BatCharg) == 0)
        {
               Serial.println(F("Error charging circuit"));
               lcd.print(F("chrg circ. fail"));
        }
      else if (digitalRead(BatFull) == 1 && digitalRead(BatCharg) == 0)
        {
               Serial.println(F("Charging"));
               lcd.print(F("    Charging    "));
        }
      else if (digitalRead(BatFull) == 0 && digitalRead(BatCharg) == 1)
        {
               Serial.println(F("Battery full"));
               lcd.print(F("  Battery full  "));
        }
      else
        {
               Serial.println(F("On battery"));
               lcd.print(F("   On battery   "));
        }
      lcd.setCursor(0, 1);
      lcd.print(F("V : "));
      int v = map(analogRead(A1), 0, 1023, 0, 859);
      lcd.print(v/100);
      lcd.print(F("."));
      lcd.print(v%100);
      lcd.setCursor(9 ,1);
      lcd.print(F("% : "));
      lcd.print(map(map(analogRead(A1), 0, 1023, 0, 859), 0,470, 0, 100));
  
      delay(200);
    }
    lcd.clear();
    beep(50);
    delay(250);
}
