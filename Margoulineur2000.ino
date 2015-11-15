/*
 * This is a free software developed by guigur aka jean margouin
 * 
 * I would not be in charge of your use of this software
 *  (c) guigur.com 2015
 */
 
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <LiquidCrystal.h>
#include <Encoder.h>

PN532_I2C pn532_i2c(Wire);
PN532 nfc(pn532_i2c);

LiquidCrystal lcd(8, 13, 12, 11, 10, 9);

Encoder myEnc(6, 5); // a changer pour 2 et 3 -> interrupts

uint8_t readLedPin = 2;
uint8_t writeLedPin = 3;
uint8_t encButton = 7;
long oldPosition  = -999;
bool buzy = false;

byte KeyA_List[][6] =
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
 // delay(1000);
  }

void menu()
{
  lcd.clear();
  lcd.print("Menu");
  lcd.setCursor(0,1);
  encoder();
  delay(10);
  if (!digitalRead(encButton))
   {
    Serial.println("BOUTON !");
    buzy = true;
   }
    //lcd.print("Waiting card");
}

void encoder()
{
  long newPosition = myEnc.read();
  if (newPosition != oldPosition)
  {
    oldPosition = newPosition;
    Serial.println(newPosition / 2);
  }
}

void loop(void)
{
 if (buzy == true)
 {
  nfc_read();
 }
 else
 {
  menu();
 }
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
           Serial.print("key used:");
              Serial.print(KeyA_List[currentblock/4][0], HEX);
              Serial.print(KeyA_List[currentblock/4][1], HEX);
              Serial.print(KeyA_List[currentblock/4][2], HEX);
              Serial.print(KeyA_List[currentblock/4][3], HEX);
              Serial.print(KeyA_List[currentblock/4][4], HEX);
              Serial.println(KeyA_List[currentblock/4][5], HEX);
          if (currentblock == 0)
          {
              success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 1, KeyA_List[0]);
              buzy = true;
          }
          else
          {
              success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 1, KeyA_List[currentblock/4]);
          }
          if (success)
          {
            authenticated = true;
          }
          else
          {
            Serial.println("Authentication error");
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
            buzy = false;
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
            buzy = false;
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
  buzy = false;
  Serial.flush();
}

void wait4button()
{
  while(digitalRead(encButton))
  {
  }
  delay(500);
}


