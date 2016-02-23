/*
 * This is a free software developed by guigur aka jean margouin
 * 
 * I would not be in charge of your use of this software
 *  (c) guigur.com 2015
 */

 /*
  * TODO:
  * Read function for the D4
  * Write function for the D4
  */

# include "margoulinade.h"

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
  { 0xFF, 0xff, 0xff, 0xff, 0xff, 0xff },  // Sector 0
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


byte KeyA_List_D4[][6] =
{

 { 0x1E, 0x14, 0x25, 0x12, 0x3D, 0x69},  // Sector 0
 { 0x1E, 0x14, 0x25, 0x12, 0x3D, 0x69},  // Sector 0
 { 0x1E, 0x14, 0x25, 0x12, 0x3D, 0x69},  // Sector 0
 { 0x1E, 0x14, 0x25, 0x12, 0x3D, 0x69},  // Sector 0
 { 0x1E, 0x14, 0x25, 0x12, 0x3D, 0x69},  // Sector 0
 { 0x1E, 0x14, 0x25, 0x12, 0x3D, 0x69},  // Sector 0
 { 0x1E, 0x14, 0x25, 0x12, 0x3D, 0x69},  // Sector 0
 { 0x1E, 0x14, 0x25, 0x12, 0x3D, 0x69},  // Sector 0
 { 0x1E, 0x14, 0x25, 0x12, 0x3D, 0x69},  // Sector 0
 { 0x1E, 0x14, 0x25, 0x12, 0x3D, 0x69},  // Sector 0
 { 0x1E, 0x14, 0x25, 0x12, 0x3D, 0x69},  // Sector 0
 { 0x1E, 0x14, 0x25, 0x12, 0x3D, 0x69},  // Sector 0
 { 0x1E, 0x14, 0x25, 0x12, 0x3D, 0x69},  // Sector 0
 { 0x1E, 0x14, 0x25, 0x12, 0x3D, 0x69},  // Sector 0
{ 0x1E, 0x14, 0x25, 0x12, 0x3D, 0x69},  // Sector 0
{ 0x1E, 0x14, 0x25, 0x12, 0x3D, 0x69},  // Sector 0
};

byte KeyB_Buffer[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

String menuStrings[][2] =
{
  {{"1. read D3"},{"read the balance of the dormitory 3"}},
  {{"2. write D3"},{"write new balance of the dormitory 3"}},
  {{"3. read D4"},{"read the balance of the dormitory 4"}},
  {{"4. write D4"},{"write new balance of the dormitory 4"}},  
};

void setup(void)
{
  pinMode(readLedPin, OUTPUT);
  pinMode(writeLedPin, OUTPUT);
  lcd.begin(16, 2);  

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
    Serial.println(newPosition / ENCODERSTEPS);
    cycleMenu = (abs(newPosition) / ENCODERSTEPS) / MENUELEMENTS;
    lcd.clear();
  }
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
    Serial.println("BOUTON !");
    switch (relativePosition)
    {
      case 0:
        nfc_read(3);
        break;
      case 1:
        lcd.clear();
        nfc_write();
        break;
      case 2:
      break;
      case 3:
      break;
    }
   }
}

int encoderWrite(byte dormitory)
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
      if (newPosition < VALMIN * ENCODERSTEPS)
        {
          myEnc.write(VALMIN * ENCODERSTEPS);
          newPosition = 0;
        }
        else if (newPosition > VALMAX * ENCODERSTEPS)
        {
          myEnc.write(VALMAX * ENCODERSTEPS);
          newPosition = 40; 
        }
        else
         {
          lcd.clear();
          lcd.print("New balance :");
          lcd.setCursor(0,1);
          lcd.print(newPosition / ENCODERSTEPS);
          
          oldPosition = newPosition;
          Serial.println(newPosition / ENCODERSTEPS);  
         } 
    }
  }
  myEnc.write(0);
  oldPosition = -1;
  return (newPosition / ENCODERSTEPS);
}

void nfc_write()
{
  uint8_t success;                          // Flag to check if there was an error with the PN532
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t currentblock;                     // Counter to keep track of which block we're on
  bool authenticated = false;               // Flag to indicate if the sector is authenticated
  uint8_t data[16]; // = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};                         // Array to store block data during reads
/**/

  int newBalance = encoderWrite(3) * 100;
  Serial.print("New value = ");Serial.println(newBalance);
  uint8_t balance[] = {0, 0};
  balance[0] = newBalance / 256;
  balance[1] = newBalance - (balance[0] * 256);
  Serial.print("Hex 1 = ");Serial.println(balance[0], HEX);
  Serial.print("Hex 2 = ");Serial.println(balance[1], HEX);


/**/
  
  digitalWrite(writeLedPin, HIGH);
  lcd.clear();
  lcd.print("WRITING ...");
  lcd.setCursor(0,1);
  lcd.print("Scan your card");
  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success)
  {
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
          if (currentblock == 24 || currentblock == 26 )
          {
           nfc.mifareclassic_ReadDataBlock(currentblock, data); 
           data[6] = balance[0];
           data[7] = balance[1];
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
          else if (currentblock == 25)
          {
           nfc.mifareclassic_ReadDataBlock(currentblock, data); 
           data[5] = balance[0];
           data[6] = balance[1];
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

  lcd.clear();
  lcd.print("New balance :");
  lcd.setCursor(0,1);
  lcd.print(newBalance / 100);
  Serial.println(newBalance, DEC);
  Serial.println("\n\nDONE ! waiting 4 the button");
  digitalWrite(writeLedPin, LOW);
  wait4button();
  lcd.clear();
  Serial.print("end read = ");Serial.println(oldPosition);
  
  
  Serial.flush();
}

void                nfc_read(byte dormitory)
{
  t_nfc_handler     nfc_handler;
  uint8_t success;                          // Flag to check if there was an error with the PN532
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t currentblock;                     // Counter to keep track of which block we're on
  bool authenticated = false;               // Flag to indicate if the sector is authenticated
  uint8_t data[16];                      // Array to store block data during reads
  
  int currentBalance = 0;

  byte KeyA_D3_part1[6] = {0xFF, 0xff, 0xff, 0xff, 0xff, 0xff};
  byte KeyA_D3_part2[6] = {0xa9, 0x6c, 0xde, 0x3f, 0x27, 0x86};
  byte KeyA_D4[6] = {0xa9, 0x6c, 0xde, 0x3f, 0x27, 0x86};

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
           
          if (currentblock >= 5 * 4 && currentblock <= 6 * 4)
          {
            for (uint8_t i = 0; i < 6; i++)
            {
                Serial.print(" "); dormitory == 3 ?
                Serial.print(KeyA_D3_part2[i], HEX) :
                Serial.print(KeyA_D4[i], HEX);
            }
              success = dormitory == 3 ?
              nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 0, KeyA_D3_part2) :
              nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 0, KeyA_D4);
              
          }
          else
          {
            for (uint8_t i = 0; i < 6; i++)
            {
                Serial.print(" "); dormitory == 3 ?
                Serial.print(KeyA_D3_part1[i], HEX) :
                Serial.print(KeyA_D4[i], HEX);
            }
              success = dormitory == 3 ?
              nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 0, KeyA_D3_part1) :
              nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 0, KeyA_D4);
          }
          Serial.println("");
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
  lcd.clear();
  lcd.print("Balance :");
  lcd.setCursor(0,1);
  lcd.print(currentBalance / 100);
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
