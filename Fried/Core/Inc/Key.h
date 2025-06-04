/*
 * Key.h
 *
 *  Created on: Jun 3, 2025
 *      Author: bensoto
 */

#ifndef INC_KEY_H_
#define INC_KEY_H_

#ifdef _KEY_C
   #define SCOPE
#else
   #define SCOPE extern
#endif




//----------------------------------

SCOPE bool KeyTapPushMap[12];

SCOPE const Key_Contorl_struct_t sKeyControl[Number_of_Keys]
#ifdef _KEY_C
={
 {PA10,0x8,PA4,0,'1',ONE_command},     // PA10 (read), PA4 (send)
 {PC0,0x4,PA4,0,'4',FOUR_command},     // PC0 (read), PA4 (send)
 {PC1,0x2,PA4,0,'7',SEVEN_command},    // PC1 (read), PA4 (send)
 {PB0,0x1,PA4,0,'*',STAR_command},     // PB0 (read), PA4 (send)

 {PA10,0x8,PA1,1,'2',TWO_command},     // PA10 (read), PA1 (send)
 {PC0,0x4,PA1,1,'5',FIVE_command},     // PC0 (read), PA1 (send)
 {PC1,0x2,PA1,1,'8',EIGHT_command},    // PC1 (read), PA1 (send)
 {PB0,0x1,PA1,1,'0',ZERO_command},     // PB0 (read), PA1 (send)

 {PA10,0x8,PA0,2,'3',THREE_command},   // PA10 (read), PA0 (send)
 {PC0,0x4,PA0,2,'6',SIX_command},      // PC0 (read), PA0 (send)
 {PC1,0x2,PA0,2,'9',NINE_command},     // PC1 (read), PA0 (send)
 {PB0,0x1,PA0,2,'#',POUND_command}     // PB0 (read), PA0 (send)
}
#endif
;




SCOPE unsigned short sKeyStatus;
SCOPE unsigned short sKeyCurrentCol[Number_of_Cols];
SCOPE unsigned short sKeyDebouncedCol[Number_of_Cols];
SCOPE unsigned short sKeyIssued;
SCOPE unsigned short sKeyPreviousCol[Number_of_Cols];
SCOPE unsigned short sKeyLow2HighCol[Number_of_Cols];



//-----------------------------------
SCOPE KEY Key_Construct(KEYNAME);
SCOPE void Key_refresh(KEY*);
bool Key_isPushed(KEY*);
SCOPE void Key_Initialize(void);
SCOPE void Keypadscan(void);
SCOPE void KeypadProcess(void);
SCOPE void AllKeyRestart(void);














#undef SCOPE
#endif /* INC_KEY_H_ */

