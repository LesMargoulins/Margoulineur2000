# include "margoulinade.h"


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
    if (EEPROM.read(eeAddress) == 0)
    {
      lcd.print(F("DISABLE"));
    }
    else
    {
         lcd.print(F("ENABLE"));
    }
    wait4button();
    if (EEPROM.read(eeAddress) == 0)
    {
      EEPROM.put(eeAddress, 1);  
    }
    else
    {
      EEPROM.put(eeAddress, 0);
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
