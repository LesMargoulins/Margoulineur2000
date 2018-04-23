/*
 * GUIGUR 2015 - 2017
 */
 
# include "margoulinade.h"

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

void    selectMenu()
{

}

void    checkPassword()
{
  lcd.clear();
  int i = 0;
  char storedpasswd[PASSWDLENGHT + 2] = "";
  Serial.print(PASSWDLENGHT);
  if (EEPROM.read(1) == 1)
  {
    lcd.print(F("Enter passwd"));
    lcd.setCursor(0, 1);
    while(i < PASSWDLENGHT)
    {
      while(digitalRead(encButton))
      {
          int newPosition = myEnc.read();
          if ((newPosition != oldPosition) )
          {
            oldPosition = newPosition;
            if(oldPosition > (PASSWDELEMENTS * ENCODERSTEPS) - 1)
            {
               myEnc.write(0);
            } 
            else if(oldPosition < 0)
            {
               myEnc.write(PASSWDELEMENTS * ENCODERSTEPS - 1);
            }
            Serial.println(newPosition / ENCODERSTEPS);
            lcd.clear();
            lcd.print(F("Enter passwd"));
          }
          lcd.setCursor(0, 1);
          lcd.print(storedpasswd);
          lcd.setCursor(i, 1);
          lcd.print(passwdChars[newPosition / ENCODERSTEPS]);
          storedpasswd[i] = passwdChars[newPosition / ENCODERSTEPS];
      }
          beep(50);
          delay(200);
        i++;
    }
    verifPasswd(storedpasswd);
  }
}

int                 verifPasswd(char storedpasswd[PASSWDLENGHT + 2])
{
  Serial.print(storedpasswd);
  lcd.clear();
  Serial.print(">");
  Serial.print(storedpasswd);
  Serial.print("<>");
  Serial.print(password);
  Serial.print("<");
  if (strcmp(storedpasswd, password) == 0)
  {
     Serial.println(F("\n\nDONE ! passwd is ok"));
     lcd.print(F("Happy hacking :)"));
     lcd.clear();
     beep(50);
     beep(50);
     beep(50);
  }
  else
  {
    Serial.println(F("\n\Wrong passwd"));
    lcd.print(F("Wrong password"));
     beep(200);
     beep(200);
     checkPassword();
  }
  delay(250);
}

void                password_enable_disble_eeprom()
{
    lcd.print(F("Password"));
    lcd.setCursor(0, 1);
    if (EEPROM.read(1) == 1)
    {
      lcd.print(F("DISABLE"));
    }
    else
    {
         lcd.print(F("ENABLE"));
    }
    wait4button();
    if (EEPROM.read(1) == 1)
    {
      EEPROM.put(1, 0);
    }
    else
    {
      EEPROM.put(1, 1);
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
