#ifndef _MARGOULINADE_H_
# define _MARGOULINADE_H_

# include <Wire.h>
# include <PN532_I2C.h>
# include <PN532.h>
# include <LiquidCrystal.h>
# include <Encoder.h>


# define OPTIMIZATION_MODE 1 // Sets the optimization, 1 to only go through the desirable blocks, 0 for all blocks

# define MENUELEMENTS 8 // Number of elements in the menu

# define ENCODERSTEPS 4 //Steps for the rotary encoder, menu selection
# define VALMIN 0 // Min value of the new balance in the write sequence
# define VALMAX 50 // Max value of the new balance in the write sequence

/* Global definition */

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

//Menu entries
String menuStrings[][2] =
		{
				{{"1. read D3"},{"read the balance of the dormitory 3"}},
				{{"2. write D3"},{"write new balance of the dormitory 3"}},
				{{"3. read D4"},{"read the balance of the dormitory 4"}},
				{{"4. write D4"},{"write new balance of the dormitory 4"}},
				{{"5. read new D4"},{"read balance new D4 cards"}},
				{{"6. write new D4"},{"write balance new D4 cards"}},
				{{"7.Format Blank to D3"},{"Convert a blank cart to a dormitory 3 card"}},
				{{"8. About"},{"by guigur & oborotev"}},
		};

typedef struct	s_nfc_handler
{
	uint8_t 	success;                          // Flag to check if there was an error with the PN532
	uint8_t 	uid[7] = {0};  					  // Buffer to store the returned UID
	uint8_t 	uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
	uint8_t		keyNumber = 0;					  // Which key type to use during authentication (0 = MIFARE_CMD_AUTH_A, 1 = MIFARE_CMD_AUTH_B) 
	uint8_t 	currentblock;                     // Counter to keep track of which block we're on
	bool 		authenticated = false;            // Flag to indicate if the sector is authenticated
	uint8_t 	data[16] = {0};                   // Array to store block data during reads
	int 		currentBalance = 0;			      // Current balance on the card
	int 		newBalance = 0;					  // New balance input by the user
	byte  		offset = 0; 					  // Byte offset
	uint8_t 	balance[2] {0}; 				  // Balance used to modify
	byte 		KeyA_D3_part1[6] = {0xFF, 0xff, 0xff, 0xff, 0xff, 0xff}; // Key to authenticate sectors different from 5 and 6, dorm 3
	byte 		KeyA_D3_part2[6] = {0xa9, 0x6c, 0xde, 0x3f, 0x27, 0x86}; // Key to authenticate sectors 5 and 6, dorm 3
	byte 		KeyA_new_D4_part1[6] = {0xa9, 0x6c, 0xde, 0x3f, 0x27, 0x86}; // Key to authenticate D4 cards type 2, first part
	byte 		KeyA_new_D4_part2[6] = {0xa0, 0x10, 0xbc, 0xde, 0x01, 0x01}; // Key to authenticate D4 cards type 2, second part
  	byte    KeyA_Blank[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; //blank key
	byte 		KeyA_D4[6]; // Key used to authenticate sectors of dorm 4
}				t_nfc_handler;

/* Functions prototypes */

//Rotary encoder control functions
void 		encoderMenu();
int 		encoderWrite();

//Washing machine nfc read/write functions
void        displayKeyDebug(byte *key);
void        writeModeInitialization(t_nfc_handler *handler, byte dormitory);
void        calcOldD4cardKeyA(t_nfc_handler *nfc_handler);
bool		guessNewD4Keys(t_nfc_handler *nfc_handler, byte *dormitory);
void 		dormitory3Authentication(t_nfc_handler *nfc_handler);
void        dormitory4newCardsAuthentication(t_nfc_handler *nfc_handler);
void        balanceReadOnly(t_nfc_handler *nfc_handler, byte dormitory);
void        balanceShow(t_nfc_handler *nfc_handler, bool mode, byte dormitory);
void        newBalanceWrite(t_nfc_handler *nfc_handler);
void 		oldDormitory4WriteBalance(t_nfc_handler *nfc_handler);
void        sectorsParsing(t_nfc_handler *nfc_handler, bool mode, byte dormitory);

# endif /* !_MARGOULINADE_H_ */
