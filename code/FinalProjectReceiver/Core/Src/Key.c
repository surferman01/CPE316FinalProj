///*
// * Key.c
// *
// *  Created on: Mar 9, 2025
// *      Author: peterh17
// *  Modified on: June 5, 2025
// *  	Ben Soto
// */
//
//
//#define _KEY_C
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
//KEY Key_Construct(KEYNAME keyname)
//{
//	KEY key;
//
//	key.Key_Name = keyname;  // This is the same name from the Key_Control_struct
//	key.isPushed = false;
//
//	return key;
//}
//
//
//void Key_refresh(KEY *key_p)
//{
//	key_p->isPushed = KeyTapPushMap[key_p->Key_Name];
//}
//
//
//bool Key_isPushed(KEY *key_p)
//{
//	return key_p->isPushed;
//}
//
//
//void Key_Initialize()
//{
//	unsigned short sIndex;
//	// Clear all debounced records, Previous, Low2High
//	for (sIndex=0; sIndex<Number_of_Cols; sIndex++)
//	{
//		sKeyDebouncedCol[sIndex] = 0x0000;
//		sKeyPreviousCol[sIndex] = 0x0000;
//		sKeyLow2HighCol[sIndex] = 0x0000;
//	}
//}
//
//
//void Keypadscan()
//{
//    unsigned short sIndex;
//    unsigned short Temp;
//
//    // Clear all key records
//    for (sIndex=0; sIndex<Number_of_Cols; sIndex++)
//    {
//      sKeyCurrentCol[sIndex] = 0x00;
//    }
//
//    // Read all 3 column
//    for (sIndex=0; sIndex<Number_of_Keys; sIndex++)
//    {
//      GPIOA->ODR &=~(PA4 | PA1 | PA0);
//      GPIOA->ODR |= sKeyControl[sIndex].sKeySend;
//      //HAL_Delay(0.5);
//
//      switch (sKeyControl[sIndex].sKeyCommand)
//	  {
//      	  case ONE_command:
//      	  case TWO_command:
//      	  case THREE_command:
//      		if (GPIOA->IDR & sKeyControl[sIndex].sKeyRead)
//      		  sKeyCurrentCol[sKeyControl[sIndex].sKeyCol]= sKeyControl[sIndex].sKeyReadTempPos;
//      		break;
//
//      	  case FOUR_command:
//      	  case FIVE_command:
//      	  case SIX_command:
//      	  case SEVEN_command:
//      	  case EIGHT_command:
//      	  case NINE_command:
//        	if (GPIOC->IDR & sKeyControl[sIndex].sKeyRead)
//        	  sKeyCurrentCol[sKeyControl[sIndex].sKeyCol] = sKeyControl[sIndex].sKeyReadTempPos;
//      	    break;
//
//      	  case STAR_command:
//      	  case ZERO_command:
//      	  case POUND_command:
//      		if (GPIOB->IDR & sKeyControl[sIndex].sKeyRead)
//      		  sKeyCurrentCol[sKeyControl[sIndex].sKeyCol] = sKeyControl[sIndex].sKeyReadTempPos;
//	  }
//    }
//
//    // Check if a key is steadily read
//    for (sIndex=0; sIndex<Number_of_Cols; sIndex++)
//    {
//      if ((sKeyCurrentCol[sIndex] == sKeyDebouncedCol[sIndex]) && (sKeyCurrentCol[sIndex] != 0x0000))
//        break;
//    }
//
//    if (sIndex <Number_of_Cols)
//    {
//    	// Check for push on/ push off (Low To High)
//    	for (sIndex=0; sIndex<Number_of_Cols; sIndex++)
//    	{
//    		Temp = sKeyCurrentCol[sIndex] ^ sKeyPreviousCol[sIndex];
//    		sKeyLow2HighCol[sIndex] = (sKeyCurrentCol[sIndex] & Temp);
//    	}
//
//    	// Update Previous records
//    	for (sIndex=0; sIndex<Number_of_Cols; sIndex++)
//    	{
//    	    sKeyPreviousCol[sIndex] = sKeyCurrentCol[sIndex];
//    	}
//
//       // Find which key is JUST depressed (Low To High)
//       for (sIndex=0 ; sIndex<Number_of_Keys; sIndex++)
//       {
//         if (sKeyLow2HighCol[sKeyControl[sIndex].sKeyCol] & sKeyControl[sIndex].sKeyReadTempPos)
//         {
//           sKeyIssued = sKeyControl[sIndex].sKeyCommand;
//           {
//              sKeyStatus |= (KeyDetect | KeyLow2High);
//              break;
//           }
//         }
//         else
//           sKeyIssued = 0xFFFF;
//       }
//    }
//    else
//    {
//      sKeyStatus &= ~(KeyDetect | KeyLow2High);
//
//      for (sIndex=0; sIndex<Number_of_Cols; sIndex++)
//       	  sKeyPreviousCol[sIndex] = 0;
//    }
//
//
//    // Transfer Current reading to debounced reading
//    for (sIndex=0; sIndex<Number_of_Cols; sIndex++)
//    {
//      sKeyDebouncedCol[sIndex] = sKeyCurrentCol[sIndex];
//      sKeyLow2HighCol[sIndex] = 0;
//    }
//}
//
//
//void KeypadProcess()
//{
//	unsigned short sIndex;
//
//	if ((sKeyStatus & KeyDetect) && (sKeyIssued != 0xFFFF))
//	{
//		switch (sKeyIssued)
//	    {
//	    	case ONE_command:
//	    	{
//	    		if (sKeyStatus & KeyLow2High)
//	    			KeyTapPushMap[ONE_command] = 1;
//	    		break;
//	    	}
//	        case FOUR_command:
//	    	{
//	    		if (sKeyStatus & KeyLow2High)
//	    			KeyTapPushMap[FOUR_command] = 1;
//	    		break;
//	    	}
//
//	        case SEVEN_command:
//	    	{
//	    		if (sKeyStatus & KeyLow2High)
//	    			KeyTapPushMap[SEVEN_command] = 1;
//
//	    		break;
//	    	}
//
//	        case STAR_command:
//	    	{
//	    		if (sKeyStatus & KeyLow2High)
//	    			KeyTapPushMap[STAR_command] = 1;
//	    		break;
//	    	}
//
//	        case TWO_command:
//	    	{
//	    		if (sKeyStatus & KeyLow2High)
//	    			KeyTapPushMap[TWO_command] = 1;
//	    		break;
//	    	}
//
//	        case FIVE_command:
//	    	{
//	    		if (sKeyStatus & KeyLow2High)
//	    			KeyTapPushMap[FIVE_command] = 1;
//
//	    		break;
//	    	}
//
//	        case EIGHT_command:
//	    	{
//	    		if (sKeyStatus & KeyLow2High)
//	    			KeyTapPushMap[EIGHT_command] = 1;
//	    		break;
//	    	}
//
//	        case ZERO_command:
//	    	{
//	    		if (sKeyStatus & KeyLow2High)
//	    			KeyTapPushMap[ZERO_command] = 1;
//	    		break;
//	    	}
//
//	        case THREE_command:
//	    	{
//	    		if (sKeyStatus & KeyLow2High)
//	    			KeyTapPushMap[THREE_command] = 1;
//	    		break;
//	    	}
//
//	        case SIX_command:
//	    	{
//	    		if (sKeyStatus & KeyLow2High)
//	    			KeyTapPushMap[SIX_command] = 1;
//	    		break;
//	    	}
//
//	        case NINE_command:
//	    	{
//	    		if (sKeyStatus & KeyLow2High)
//	    			KeyTapPushMap[NINE_command] = 1;
//	    		break;
//	    	}
//
//	        case POUND_command:
//	        {
//	        	if (sKeyStatus & KeyLow2High)
//	    			KeyTapPushMap[POUND_command] = 1;
//	        	break;
//	        }
//
//            default:
//            	break;
//        }
//
//		sKeyStatus &= ~(KeyDetect | KeyLow2High );
//
//		// Clear all Low-2-High and High-2-Low records
//		for (sIndex=0; sIndex<Number_of_Cols; sIndex++)
//		  sKeyLow2HighCol[sIndex] = 0x0000;
//	}
//}
//
//void AllKeyRestart()
//{
//	uint16_t i;
//	for (i=0; i<12; i++)
//		KeyTapPushMap[i] = false;
//}
//
