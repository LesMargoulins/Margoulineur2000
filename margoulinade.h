#ifndef _MARGOULINADE_H_
# define _MARGOULINADE_H_

# include <Wire.h>
# include <PN532_I2C.h>
# include <PN532.h>
# include <LiquidCrystal.h>
# include <Encoder.h>

# define MENUELEMENTS 4 // Number of elements in the menu

# define ENCODERSTEPS 2 //Steps for the rotary encoder, menu selection
# define VALMIN 0 // Min value of the new balance in the write sequence
# define VALMAX 40 // Max value of the new balance in the write sequence 

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
	byte 		KeyA_D4[6] = {0xa9, 0x6c, 0xde, 0x3f, 0x27, 0x86}; // Key used to authenticate sectors of dorm 4
}				t_nfc_handler;

# endif /* !_MARGOULINADE_H_ */
