/*
 * This is a free software developed by guigur aka jean margouin
 * 
 * I would not be in charge of your use of this software
 *  (c) guigur.com 2015 - 2017
 */

# include "margoulinade.h"

  const int numReadings = 20;  
  int readings[numReadings];      // the readings from the analog input
  int readIndex = 0;              // the index of the current reading
  int total = 0;                  // the running total
  int average = 0;
  
void setup(void)
{
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
  {
    readings[thisReading] = 0;
  }
  pinMode(readLedPin, OUTPUT);
  pinMode(writeLedPin, OUTPUT);

  //  pinMode(BatSense);
  pinMode(BatFull, INPUT_PULLUP);
  pinMode(BatCharg, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  digitalWrite(BatFull, INPUT_PULLUP);
  digitalWrite(BatCharg, INPUT_PULLUP);
  
  lcd.begin(16, 2);

  pinMode(encButton, INPUT_PULLUP);
  lcd.print(F("Margoulineur2000"));
  
  Serial.begin(115200);
  Serial.println(F("Looking for PN532..."));

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();

  if (digitalRead(encButton) == LOW)
    silentMode = true;
    
  if (!versiondata)
  {
    beep(200);
    beep(200);
    Serial.print(F("Didn't find PN53x board"));
    lcd.setCursor(0,1);
    lcd.print(F("NFC MODULE ERROR"));
    while (1);
  }
  beep(50);
  beep(50);
  beep(50);
  Serial.print(F("Found chip PN5")); Serial.println((versiondata>>24) & 0xFF, HEX);
  Serial.print(F("Firmware ver. ")); Serial.print((versiondata>>16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  nfc.SAMConfig(); // configure board to read RFID tags
  
  lcd.setCursor(0,1);
  lcd.print(F("NFC OK"));
  delay(1000);
  lcd.clear();
  checkMem();
  checkPassword();
  //eepromDebug();
}

void loop(void)
{
 byte relativePosition = (abs(oldPosition) / ENCODERSTEPS) - (cycleMenu * MENUELEMENTS);
  lcd.setCursor(0,0);
  lcd.print(menuStrings[relativePosition][0]);
  lcd.setCursor(0,1);
  lcd.print(menuStrings[relativePosition][1]);
  encoderMenu();
  if (!digitalRead(encButton))
   {
    Serial.println(F("BOUTON !"));
    switch (relativePosition)
    {
      case 0:
        lcd.clear();
        beep(50);
        nfc_read_write(3, false);
        break;
      case 1:
        lcd.clear();
        beep(50);
        nfc_read_write(3, true);
        break;
      case 2:
        lcd.clear();
        beep(50);
        nfc_read_write(4, false);
        break;
      case 3:
        lcd.clear();
        beep(50);
        nfc_read_write(4, true);
        break;
      case 4:
        lcd.clear();
        beep(50);
        nfc_read_write(5, false);
        break;
      case 5:
        lcd.clear();
        beep(50);
        nfc_read_write(5, true);
        break;
      case 6:
        lcd.clear();
        beep(50);
        batStatus();
        break;
      case 7:
        lcd.clear();
        beep(50);
        about();
        break;
      case 8:
        lcd.clear();
        beep(50);
        buzzer_eeprom();
        break;
      case 9:
        lcd.clear();
        beep(50);
        buzzer_level_eeprom();
        break;
      case 10:
        lcd.clear();
        beep(50);
        password_enable_disble_eeprom();
        break;
      case 11:
        lcd.clear();
        beep(50);
        break;
    }
   }
}

void                nfc_read_write(byte dormitory, bool mode)
{
  t_nfc_handler     nfc_handler;

  if (mode) // We are in write mode
    writeModeInitialization(&nfc_handler, dormitory);

  mode? digitalWrite(writeLedPin, HIGH) : digitalWrite(readLedPin, HIGH);
  lcd.clear();
  mode ? lcd.print(F("WRITING ...")) : lcd.print(F("READING ..."));
  lcd.setCursor(0,1);
  lcd.print(F("Scan your card"));
  
  nfc_handler.success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, nfc_handler.uid, &nfc_handler.uidLength);

  if (nfc_handler.success) {
    // Display some basic information about the card
    Serial.println(F("Found an ISO14443A card"));
    lcd.clear();
    lcd.print(F("I Found a card !"));
    Serial.print(F("  UID Length: "));
    Serial.print(nfc_handler.uidLength, DEC);
    Serial.println(F(" bytes"));
    Serial.print(F("  UID Value: "));
    for (uint8_t i = 0; i < nfc_handler.uidLength; i++)
    {
      Serial.print(nfc_handler.uid[i], HEX);
      Serial.print(' ');
    }
    Serial.println("");

    if (nfc_handler.uidLength == 4)
    {
      Serial.println(F("Seems to be a Mifare Classic card (4 byte UID)"));
        if (dormitory == 4)
            calcOldD4cardKeyA(&nfc_handler);
        else if (dormitory == 5)
        {
            if (!guessNewD4Keys(&nfc_handler, &dormitory))
                return ;
        }
        sectorsParsing(&nfc_handler, mode, dormitory);
    }
    else
      Serial.println(F("Ooops ... this doesn't seem to be a Mifare Classic card!"));
  }
  lcd.clear();
  lcd.setCursor(0,0);
  balanceShow(&nfc_handler, mode, dormitory);
  wait4button();
  Serial.flush();
}
