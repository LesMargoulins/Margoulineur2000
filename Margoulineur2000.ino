/*
 * This is a free software developed by guigur aka jean margouin
 * 
 * I would not be in charge of your use of this software
 *  (c) guigur.com 2015
 */

 /*
  * TODO:
  * 
  */
  
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <LiquidCrystal.h>
#include <Encoder.h>

#define MENUELEMENTS 2

#define VALMIN 0
#define VALMAX 40

PN532_I2C pn532_i2c(Wire);
PN532 nfc(pn532_i2c);

LiquidCrystal lcd(8, 13, 12, 11, 10, 9);

Encoder myEnc(2, 3);

uint8_t readLedPin = 7; //rouge
uint8_t writeLedPin = 6; //vert
uint8_t encButton = 5;
long oldPosition  = 0;
long cursorPosition  = 0;


int cycleMenu = 0;

byte KeyA_List_D3[][6] =
{
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },  // Sector 0
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },  // Sector 1
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },  // Sector 2
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },  // Sector 3
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },  // Sector 4
  { 0xa9, 0x6c, 0xde, 0x3f, 0x27, 0x86 },  // Sector 5
  { 0xa9, 0x6c, 0xde, 0x3f, 0x27, 0x86 },  // Sector 6
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },  // Sector 7
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },  // Sector 8
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },  // Sector 9
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },  // Sector 10
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },  // Sector 11
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },  // Sector 12
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },  // Sector 13
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },  // Sector 14
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },  // Sector 15
};

String menuStrings[][2] =
{
  {{"1. read card"},{"read the balance"}},
  {{"2. add credits"},{"write new balance"}},
};

void setup(void)
{
  pinMode(readLedPin, OUTPUT);
  pinMode(writeLedPin, OUTPUT); lcd.begin(16, 2);  

  pinMode(encButton, INPUT_PULLUP);
  lcd.print("Margoulineur2000");
  
  Serial.begin(115200);
  Serial.println("Looking for PN532...");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  
  if (! versiondata)
  {
    Serial.print("Didn't find PN53x board");
    lcd.setCursor(0,1);
    lcd.print("NFC MODULE ERROR");
    while (1);
  }
  
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  nfc.SAMConfig(); // configure board to read RFID tags
  
  lcd.setCursor(0,1);
  lcd.print("NFC OK");
  delay(1000);
  lcd.clear();
  }



void encoderMenu()
{
  int newPosition = myEnc.read();
  if ((newPosition != oldPosition) )
  {
    oldPosition = newPosition;
    if(oldPosition < 0)
    {
       myEnc.write(255);
    }
    if(oldPosition > 255)
    {
       myEnc.write(0);
    }
    Serial.println(newPosition / 4);
    cycleMenu = (abs(newPosition) / 4) / MENUELEMENTS;
    lcd.clear();
  }
}

void loop(void)
{
 byte relativePosition = (abs(oldPosition) / 4) - (cycleMenu * MENUELEMENTS);
  lcd.setCursor(0,0);
  lcd.print(menuStrings[relativePosition][0]);
  lcd.setCursor(0,1);
  lcd.print(menuStrings[relativePosition][1]);
  encoderMenu();
  if (!digitalRead(encButton))
   {
    Serial.println("BOUTON !");
    if (relativePosition == 0)
      nfc_read();
    else if (relativePosition == 1)
    {
      lcd.clear();
      nfc_write();
    }
   }
}

int encoderWrite()
{
  int newPosition;
  myEnc.write(0);
  Serial.print("jeej");
  delay(500);
  oldPosition = -1;
  while(digitalRead(encButton))
  {
    newPosition = myEnc.read();
    if ((newPosition != oldPosition) )
    {
      if (newPosition < VALMIN * 4)
        {
          myEnc.write(VALMIN * 4);
          newPosition = 0;
        }
        else if (newPosition > VALMAX * 4)
        {
          myEnc.write(VALMAX * 4);
          newPosition = 40; 
        }
        else
         {
          lcd.clear();
          lcd.print("New Balance :");
          lcd.print(newPosition / 4);
          oldPosition = newPosition;
          Serial.println(newPosition / 4);  
         } 
    }
  }
  myEnc.write(0);
  oldPosition = -1;
  return (newPosition / 4);
}

void nfc_write()
{
  uint8_t success;                          // Flag to check if there was an error with the PN532
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t currentblock;                     // Counter to keep track of which block we're on
  bool authenticated = false;               // Flag to indicate if the sector is authenticated
  uint8_t data[16] = {0x04, 0x0A, 0x52, 0x18, 0x7B, 0x2C, 0x10, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00, 0x45, 0x55, 0x43};                         // Array to store block data during reads
  uint8_t savedData[16];      
  int currentBalance = 0;

  int value = encoderWrite() * 100;
  
  
  Serial.print("New value = ");Serial.println(value);
  uint8_t balance[] = {0, 0};
  balance[0] = value / 256;
  balance[1] = value - (balance[0] * 256);
  Serial.print("Hex 1 = ");Serial.println(balance[0], HEX);
  Serial.print("Hex 2 = ");Serial.println(balance[1], HEX);
  digitalWrite(writeLedPin, HIGH);
  lcd.clear();
  lcd.print("WRITING ...");
  lcd.setCursor(0,1);
  lcd.print("Scan your card");
  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    lcd.clear();
    lcd.print("I Found a card !");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(uid[i], HEX);
      Serial.print(' ');
    }
    Serial.println("");

    if (uidLength == 4)
    {
      Serial.println("Seems to be a Mifare Classic card (4 byte UID)");

      // Now we try to go through all 16 sectors (each having 4 blocks)
      // authenticating each sector, and then dumping the blocks
      for (currentblock = 0; currentblock < 64; currentblock++)
      {
        // Check if this is a new block so that we can reauthenticate
        if (nfc.mifareclassic_IsFirstBlock(currentblock)) authenticated = false;

        // If the sector hasn't been authenticated, do so first
        if (!authenticated)
        {
          // Starting of a new sector ... try to to authenticate
          Serial.print("------------------------Sector ");Serial.print(currentblock/4, DEC);Serial.println("-------------------------");
           Serial.print("keys used:");
           for (uint8_t i = 0; i < 5; i++)
           {
            Serial.print(" ");Serial.print(KeyA_List_D3[currentblock/4][i], HEX);
           }
           Serial.println("");
           
          if (currentblock == 0)
          {
              success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 1, KeyA_List_D3[0]);     
          }
          else
          {
              success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 1, KeyA_List_D3[currentblock/4]);
          }
          if (success)
          {
            authenticated = true;
          }
          else
          {
            Serial.println("Authentication error");
            lcd.clear();
            lcd.print("key error");
          }
        }
        if (!authenticated)
        {
          Serial.print("Block ");Serial.print(currentblock, DEC);Serial.println(" unable to authenticate");
        }
        else
        {
          if (currentblock == 24)
          {
            success = nfc.mifareclassic_WriteDataBlock(currentblock, data);
            if (success)
            {
              Serial.print("Block ");Serial.print(currentblock, DEC);
              if (currentblock < 10)
              {
                Serial.print("  ");
              }
              else
              {
                Serial.print(" ");
              }
              nfc.PrintHexChar(data, 16);
              
            }
          }
          else
          {
            // Oops ... something happened
            Serial.print("Block ");Serial.print(currentblock, DEC);
            Serial.println(" unable to read this block");
            
          }
        }
      }
    }
    else
    {
      Serial.println("Ooops ... this doesn't seem to be a Mifare Classic card!");
    }
  }
  // Wait a bit before trying again
//  Serial.println(savedData[0], HEX);
 // Serial.println(savedData[1], HEX);
  
  lcd.clear();
  lcd.print("Balance :");
  lcd.setCursor(0,1);
  lcd.print(currentBalance);
  Serial.println(currentBalance, DEC);
  Serial.println("\n\nDONE ! waiting 4 the button");
  digitalWrite(writeLedPin, LOW);
  wait4button();
  lcd.clear();
  Serial.print("end read = ");Serial.println(oldPosition);
  
  
  Serial.flush();
}

void nfc_read()
{
  uint8_t success;                          // Flag to check if there was an error with the PN532
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t currentblock;                     // Counter to keep track of which block we're on
  bool authenticated = false;               // Flag to indicate if the sector is authenticated
  uint8_t data[16];                         // Array to store block data during reads
  uint8_t savedData[16];      
  int currentBalance = 0;

  digitalWrite(readLedPin, HIGH);
  lcd.clear();
  lcd.print("READING ...");
  lcd.setCursor(0,1);
  lcd.print("Scan your card");
  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    lcd.clear();
    lcd.print("I Found a card !");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(uid[i], HEX);
      Serial.print(' ');
    }
    Serial.println("");

    if (uidLength == 4)
    {
      Serial.println("Seems to be a Mifare Classic card (4 byte UID)");

      // Now we try to go through all 16 sectors (each having 4 blocks)
      // authenticating each sector, and then dumping the blocks
      for (currentblock = 0; currentblock < 64; currentblock++)
      {
        // Check if this is a new block so that we can reauthenticate
        if (nfc.mifareclassic_IsFirstBlock(currentblock)) authenticated = false;

        // If the sector hasn't been authenticated, do so first
        if (!authenticated)
        {
          // Starting of a new sector ... try to to authenticate
          Serial.print("------------------------Sector ");Serial.print(currentblock/4, DEC);Serial.println("-------------------------");
           Serial.print("keys used:");
           for (uint8_t i = 0; i < 5; i++)
           {
            Serial.print(" ");Serial.print(KeyA_List_D3[currentblock/4][i], HEX);
           }
           Serial.println("");
           
          if (currentblock == 0)
          {
              success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 1, KeyA_List_D3[0]);
              
          }
          else
          {
              success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 1, KeyA_List_D3[currentblock/4]);
          }
          if (success)
          {
            authenticated = true;
          }
          else
          {
            Serial.println("Authentication error");
            lcd.clear();
            lcd.print("key error");
          }
        }
        // If we're still not authenticated just skip the block
        if (!authenticated)
        {
          Serial.print("Block ");Serial.print(currentblock, DEC);Serial.println(" unable to authenticate");
        }
        else
        {
          // Authenticated ... we should be able to read the block now
          // Dump the data into the 'data' array
          success = nfc.mifareclassic_ReadDataBlock(currentblock, data);
          if (success)
          {
            // Read successful
            Serial.print("Block ");Serial.print(currentblock, DEC);
            if (currentblock < 10)
            {
              Serial.print("  ");
            }
            else
            {
              Serial.print(" ");
            }
            // Dump the raw data
            nfc.PrintHexChar(data, 16);
            
            if (currentblock == 24)
            {
              currentBalance = (data[6] * 256) + data[7];
            }
          }
          else
          {
            // Oops ... something happened
            Serial.print("Block ");Serial.print(currentblock, DEC);
            Serial.println(" unable to read this block");
            
          }
        }
      }
    }
    else
    {
      Serial.println("Ooops ... this doesn't seem to be a Mifare Classic card!");
    }
  }
  // Wait a bit before trying again
//  Serial.println(savedData[0], HEX);
 // Serial.println(savedData[1], HEX);
  
  lcd.clear();
  lcd.print("Balance :");
  lcd.setCursor(0,1);
  lcd.print(currentBalance);
  Serial.println(currentBalance, DEC);
  Serial.println("\n\nDONE ! waiting 4 the button");
  digitalWrite(readLedPin, LOW);
  wait4button();
  lcd.clear();
  Serial.print("end read = ");Serial.println(oldPosition);
  
  
  Serial.flush();
}

void wait4button()
{
  while(digitalRead(encButton))
  {
  }
  delay(500);
}


