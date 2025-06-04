/*
 * HAL.h
 *
 *  Created on: Jun 3, 2025
 *      Author: bensoto
 */

#ifndef INC_HAL_H_
#define INC_HAL_H_


#ifdef _HAL_C
   #define SCOPE
#else
   #define SCOPE extern
#endif


struct _HAL
{
	KEY key1;
	KEY key2;
	KEY key3;
	KEY key4;
	KEY key5;
	KEY key6;
	KEY key7;
	KEY key8;
	KEY key9;
	KEY key0;
	KEY keyS;
	KEY keyP;
};
typedef struct _HAL HAL;

//-----------------------------------------------------


//----------------------------------------------------
SCOPE void HAL_refresh(HAL*);
SCOPE HAL HAL_construct(void);

#undef SCOPE
#endif /* INC_HAL_H_ */
