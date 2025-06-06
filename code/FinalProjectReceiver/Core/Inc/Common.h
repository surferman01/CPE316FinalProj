/*
 * Common.h
 *
 *  Created on: Mar 10, 2025
 *      Author: peterh17
 */

#ifndef INC_COMMON_H_
#define INC_COMMON_H_

#define KeyTapRecord 0

#define RELEASED 0
#define PUSHED 1
#define PRESSED 1

#define Number_of_Keys 12
#define Number_of_Cols  3
#define PA0 0x0001
#define PA1 0x0002
#define PA4 0x0010
#define PB0 0x0001
#define PC1 0x0002
#define PC0 0x0001
#define PA10 0x0400

#define KeyDetect 	0x0001
#define KeyLow2High 0x0002

enum _KeyName
{
  ONE_command,
  FOUR_command,
  SEVEN_command,
  STAR_command,
  TWO_command,
  FIVE_command,
  EIGHT_command,
  ZERO_command,
  THREE_command,
  SIX_command,
  NINE_command,
  POUND_command
};
typedef enum _KeyName KEYNAME;


struct _KEY
{
	KEYNAME Key_Name;  // This is the same name from the Key_Contorl_struct
	bool isPushed;
	int	pushState;
};

typedef struct _KEY KEY;


typedef struct
{
   unsigned short sKeyRead;
   unsigned short sKeyReadTempPos;
   unsigned short sKeySend;
   unsigned short sKeyCol;
   char KeyLetter;
   unsigned short sKeyCommand;
} Key_Contorl_struct_t;

//--------------------------------------------
#undef SCOPE
#endif /* INC_COMMON_H_ */
