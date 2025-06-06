///*
// * HAL.c
// *
// *  Created on: Mar 9, 2025
// *      Author: peterh17
// *  Modified on: June 5, 2025
// *  	Ben Soto
// */
//
//
//#define _HAL_C
//
//#include <stdbool.h>
//#include "Common.h"
//#include "HAL.h"
//#include "Key.h"
//#include "Timer.h"
//#include "main.h"
//#include <string.h>
//#include <stdio.h>
//
//
//
//void HAL_refresh(HAL *hal_p)
//{
//	Key_refresh(&hal_p->key1);
//	Key_refresh(&hal_p->key2);
//	Key_refresh(&hal_p->key3);
//	Key_refresh(&hal_p->key4);
//	Key_refresh(&hal_p->key5);
//	Key_refresh(&hal_p->key6);
//	Key_refresh(&hal_p->key7);
//	Key_refresh(&hal_p->key8);
//	Key_refresh(&hal_p->key9);
//	Key_refresh(&hal_p->key0);
//	Key_refresh(&hal_p->keyS);
//	Key_refresh(&hal_p->keyP);
//}
//
//
//HAL HAL_construct()
//{
//	// The HAL object will be returned at the end of construction
//	HAL hal;
//
//	hal.key1 = Key_Construct(ONE_command);
//	hal.key2 = Key_Construct(TWO_command);
//	hal.key3 = Key_Construct(THREE_command);
//	hal.key4 = Key_Construct(FOUR_command);
//	hal.key5 = Key_Construct(FIVE_command);
//	hal.key6 = Key_Construct(SIX_command);
//	hal.key7 = Key_Construct(SEVEN_command);
//	hal.key8 = Key_Construct(EIGHT_command);
//	hal.key9 = Key_Construct(NINE_command);
//	hal.key0 = Key_Construct(ZERO_command);
//	hal.keyS = Key_Construct(STAR_command);
//	hal.keyP = Key_Construct(POUND_command);
//
//	return hal;
//}
