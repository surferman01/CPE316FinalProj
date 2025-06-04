/*
 * Display.c
 *
 *  Created on: Mar 9, 2025
 *      Author: peterh17
 */


#define _DISPLAY_C

#include <stdbool.h>
#include "Common.h"
#include "main.h"
#include "HAL.h"
#include "Key.h"
#include "Display.h"
#include "App_KeyDisplay.h"
#include "Timer.h"
#include <string.h>
#include <stdio.h>

void DisplayClear(UART_HandleTypeDef *huart2_p)
{
	 HAL_UART_Transmit(huart2_p, (uint8_t *)clrscr, strlen(clrscr), HAL_MAX_DELAY);
}


void DisplayHome(UART_HandleTypeDef *huart2_p)
{
	 HAL_UART_Transmit(huart2_p, (uint8_t *)cursorHome, strlen(cursorHome), HAL_MAX_DELAY);
}

void DisplayString(UART_HandleTypeDef *huart2_p, char *string)
{
	 HAL_UART_Transmit(huart2_p, (uint8_t *)string, strlen(string), HAL_MAX_DELAY);
}



