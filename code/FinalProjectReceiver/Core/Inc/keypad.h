#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

/** keypad.h
 * Implements keypad scanning with de-bouncing and repeating
 * Timer lengths are set in timer.h
 * 
 * Created by Alex Castellar
 * Original code from Professor Han
 */

#ifdef _KEYPAD_C
	#define SCOPE
#else
	#define SCOPE extern
#endif

// Keypad functions

SCOPE void KeypadClear();
SCOPE void KeypadScan();
SCOPE void KeyProcess(void (*keyPressedCallback)(char key), void (*keyRepeatedCallback)(char key), void (*keyReleasedCallback)(char key));

// Keypad settings

#define Number_of_Keys 12
#define Number_of_Cols  3
#define PA0 0x0001
#define PA1 0x0002
#define PA4 0x0010
#define PB0 0x0001
#define PC1 0x0002
#define PC0 0x0001
#define PA10 0x0400

#define KeyDetect 			0x0001
#define KeyLow2High 		0x0002
#define KeyHigh2Low 		0x0004
#define KeyIsOn				0x0008
#define KeyIsOff			0x0010
#define KeyRepeat			0x0020  // after a key is pressed & held for one second
#define KeyToBeRepeated 	0x0040

typedef struct
{
   unsigned short sKeyRead;
   unsigned short sKeyReadTempPos;
   unsigned short sKeySend;
   unsigned short sKeyCol;
   char KeyLetter;
} Key_Control_struct_t;

#ifdef _KEYPAD_C
const Key_Control_struct_t sKeyControl[Number_of_Keys] =
{
 {PA10,0x8,PA4,0,'1'},     // PA10 (read), PA4 (send)
 {PC0,0x4,PA4,0,'4'},     // PC0 (read), PA4 (send)
 {PC1,0x2,PA4,0,'7'},    // PC1 (read), PA4 (send)
 {PB0,0x1,PA4,0,'*'},     // PB0 (read), PA4 (send)

 {PA10,0x8,PA1,1,'2'},     // PA10 (read), PA1 (send)
 {PC0,0x4,PA1,1,'5'},     // PC0 (read), PA1 (send)
 {PC1,0x2,PA1,1,'8'},    // PC1 (read), PA1 (send)
 {PB0,0x1,PA1,1,'0'},     // PB0 (read), PA1 (send)

 {PA10,0x8,PA0,2,'3'},   // PA10 (read), PA0 (send)
 {PC0,0x4,PA0,2,'6'},      // PC0 (read), PA0 (send)
 {PC1,0x2,PA0,2,'9'},     // PC1 (read), PA0 (send)
 {PB0,0x1,PA0,2,'#'}     // PB0 (read), PA0 (send)
};
#else
extern const Key_Control_struct_t sKeyControl[Number_of_Keys];
#endif

// Keypad variables

SCOPE unsigned short sKeyStatus;
SCOPE unsigned short sKeyCurrentCol[Number_of_Cols];
SCOPE unsigned short sKeyDebouncedCol[Number_of_Cols];
SCOPE unsigned short sKeyIssued;
SCOPE unsigned short sKeyReleased;
SCOPE unsigned short sIndexCopy;
SCOPE unsigned short sKeyPushedRecord;

SCOPE unsigned short sKeyPreviousCol[Number_of_Cols];
SCOPE unsigned short sKeyLow2HighCol[Number_of_Cols];
SCOPE unsigned short sKeyHigh2LowCol[Number_of_Cols];

SCOPE void key1();
SCOPE void key2();
SCOPE void key3();
SCOPE void key4();
SCOPE void key5();
SCOPE void key6();
SCOPE void key7();
SCOPE void key8();
SCOPE void key9();
SCOPE void key0();
SCOPE void keyS();
SCOPE void keyP();


#undef SCOPE
#endif /* INC_KEYPAD_H_ */
