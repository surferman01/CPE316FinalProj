/*
 * App_KeyDisplay.c
 *
 *  Created on: Mar 9, 2025
 *      Author: peterh17
 */

#define _APP_KEYDISPLAY_C

#include <stdbool.h>
#include "Common.h"
#include "main.h"
#include "Key.h"
#include "HAL.h"
#include "App_KeyDisplay.h"
#include "Timer.h"
#include "Display.h"
#include <string.h>
#include <stdio.h>


App_KeyDisplay App_KeyDisplay_construct()
{
	// This is the constructor for the application. It initializes the state, the timer
	//  associates with the state and the variables used in the application.
	App_KeyDisplay app;

	app.app_timer = &sTimer[APP_SCREEN_Timer];
	app.state = INTRO_SCREEN;
	*app.app_timer = INTRO_SCREEN_WAIT;

	app.Numbers_Keys_Pushed = 0;
	app.Sum_of_Numbers_Pushed = 0;

	return app;
}

void App_KeyDisplay_loop(App_KeyDisplay *app_p, HAL *hal_p, UART_HandleTypeDef *huart2_p)
{
	switch (app_p->state)
	{
		case INTRO_SCREEN:
			App_KeyDisplay_handleIntroScreen(app_p, huart2_p);
			break;

		case PROCESS_SCREEN:
			App_KeyDisplay_HandleProcessScreen(app_p, hal_p, huart2_p);
			break;

		case RESULT_SCREEN:
			App_KeyDisplay_HandleResultScreen(app_p, hal_p, huart2_p);
			break;

		case EXIT_SCREEN:
			while(1)
			;;

	}
}


/* Callback function for when the app is in the Intro Screen state.
 * It's used to break down the app function into smaller sub functions.
 */

void App_KeyDisplay_ShowIntroScreen(UART_HandleTypeDef *huart2_p)
{
	char *string1 = "This is a demo system.\n\r";
	char *string2 = "----------------------\n\r";
	char *string3 = "It will count how many keys are pushed, and calculate their sum.\n\n\r";
	char *string4 = "Starting in 5 seconds. ";

	DisplayClear(huart2_p);
	DisplayHome(huart2_p);
	DisplayString(huart2_p, string1);
	DisplayString(huart2_p, string2);
	DisplayString(huart2_p, string3);
	DisplayString(huart2_p, string4);
}



void App_KeyDisplay_handleIntroScreen(App_KeyDisplay *app_p, UART_HandleTypeDef *huart2_p)
{
	if (*app_p->app_timer == 0)
	{
		app_p->state = PROCESS_SCREEN;
		App_KeyDisplay_ShowProcessScreen(app_p, huart2_p);
	}
}


void App_KeyDisplay_ShowProcessScreen(App_KeyDisplay *app_p, UART_HandleTypeDef *huart2_p)
{
	char *string1 = "This demo calculates two numbers.\n\r";
	char *string2 = "---------------------------------\n\r";
	char *string3 = "(1). Count how many keys are pushed.\n\r";
	char *string4 = "(2). Calculate the sum from all pushed keys.\n\n\r";
	char *string5 = "Please '*' key to continue or '#' to exit at the end.\n\r";

	DisplayClear(huart2_p);
	DisplayHome(huart2_p);
	DisplayString(huart2_p, string1);
	DisplayString(huart2_p, string2);
	DisplayString(huart2_p, string3);
	DisplayString(huart2_p, string4);
	DisplayString(huart2_p, string5);

}


void App_KeyDisplay_ShowProcessScreen2(App_KeyDisplay *app_p, HAL *hal_p, UART_HandleTypeDef *huart2_p)
{
	// Transition to start counting how many keys and sum of keys
	if (Key_isPushed(&hal_p->keyS))
	{
		// clear all low2high flags
		AllKeyRestart();

		// Update internal logic state
		app_p->state = PROCESS_SCREEN;

		DisplayClear(huart2_p);
		DisplayHome(huart2_p);
		Star_first_press = 0;		// Reset the star flag

		// Enter any number from the keypad or '#' key to end
		DisplayString(huart2_p, "\n\r Push any key from the keypad.\n\r");
		DisplayString(huart2_p, "Enter '#' key to end and to see calculations.\n\n\r");
	}
	else if (Key_isPushed(&hal_p->keyP))
	{
		app_p->state = EXIT_SCREEN;
		AllKeyRestart();
	}
}

void App_KeyDisplay_HandleProcessScreen(App_KeyDisplay *app_p, HAL *hal_p, UART_HandleTypeDef *huart2_p)
{

	char NumberCount1[50], NumberCount2[50];
	char *string1 = "Exiting in 6 seconds... ";

	if (Star_first_press == 1)
	{
	  if (Key_isPushed(&hal_p->key1))
	  {
		  DisplayString(huart2_p, "1 is pushed.\n\r");
		  app_p->Numbers_Keys_Pushed++;
		  app_p->Sum_of_Numbers_Pushed += 1;
		  AllKeyRestart();
	  }
	  else if (Key_isPushed(&hal_p->key2))
	  {
		  DisplayString(huart2_p, "2 is pushed.\n\r");
		  app_p->Numbers_Keys_Pushed++;
		  app_p->Sum_of_Numbers_Pushed += 2;
		  AllKeyRestart();
	  }
	  else if (Key_isPushed(&hal_p->key3))
	  {
		  DisplayString(huart2_p, "3 is pushed.\n\r");
		  app_p->Numbers_Keys_Pushed++;
		  app_p->Sum_of_Numbers_Pushed += 3;
		  AllKeyRestart();
	  }
	  else if (Key_isPushed(&hal_p->key4))
	  {
		  DisplayString(huart2_p, "4 is pushed.\n\r");
		  app_p->Numbers_Keys_Pushed++;
		  app_p->Sum_of_Numbers_Pushed += 4;
		  AllKeyRestart();
	  }
	  else if (Key_isPushed(&hal_p->key5))
	  {
		  DisplayString(huart2_p, "5 is pushed.\n\r");
		  app_p->Numbers_Keys_Pushed++;
		  app_p->Sum_of_Numbers_Pushed += 5;
		  AllKeyRestart();
	  }
	  else if (Key_isPushed(&hal_p->key6))
	  {
		  DisplayString(huart2_p, "6 is pushed.\n\r");
		  app_p->Numbers_Keys_Pushed++;
		  app_p->Sum_of_Numbers_Pushed += 6;
		  AllKeyRestart();
	  }
	  else if (Key_isPushed(&hal_p->key7))
	  {
		  DisplayString(huart2_p, "7 is pushed.\n\r");
		  app_p->Numbers_Keys_Pushed++;
		  app_p->Sum_of_Numbers_Pushed += 7;
		  AllKeyRestart();
	  }
	  else if (Key_isPushed(&hal_p->key8))
	  {
		  DisplayString(huart2_p, "8 is pushed.\n\r");
		  app_p->Numbers_Keys_Pushed++;
		  app_p->Sum_of_Numbers_Pushed += 8;
		  AllKeyRestart();
	  }
	  else if (Key_isPushed(&hal_p->key9))
	  {
		  DisplayString(huart2_p, "9 is pushed.\n\r");
		  app_p->Numbers_Keys_Pushed++;
		  app_p->Sum_of_Numbers_Pushed += 9;
		  AllKeyRestart();
	  }
	  else if (Key_isPushed(&hal_p->key0))
	  {
		  DisplayString(huart2_p, "0 is pushed.\n\r");
		  app_p->Numbers_Keys_Pushed++;
		  AllKeyRestart();
	  }
	  else if (Key_isPushed(&hal_p->keyS))
	  {
		  DisplayString(huart2_p, "* is pushed.\n\r");
		  app_p->Numbers_Keys_Pushed++;
		  AllKeyRestart();
	  }
	  else if (Key_isPushed(&hal_p->keyP))
	  {
	  		DisplayClear(huart2_p);
	  		DisplayHome(huart2_p);
	  		sprintf(NumberCount1, "Keys entered: %d.\n\r",app_p->Numbers_Keys_Pushed);
	  		DisplayString(huart2_p, NumberCount1);
	  		sprintf(NumberCount2, "Sum of entered keys: %d.\n\n\n\r",app_p->Sum_of_Numbers_Pushed);
	  		DisplayString(huart2_p, NumberCount2);

	  		HAL_UART_Transmit(huart2_p, (uint8_t *)string1, strlen(string1), HAL_MAX_DELAY);

	  		app_p->state = RESULT_SCREEN;
	  		*app_p->app_timer = RESULT_SCREEN_WAIT;
	  	    AllKeyRestart();
	  }

	}
	else
	{
	  if (Key_isPushed(&hal_p->keyS))
	  {
	     Star_first_press = 1;
		 HAL_UART_Transmit(huart2_p, (uint8_t *)"Start...\n\n\r", 11, HAL_MAX_DELAY);
		 AllKeyRestart();
	  }
	}
}


void App_KeyDisplay_HandleResultScreen(App_KeyDisplay *app_p, HAL *hal_p, UART_HandleTypeDef *huart2_p)
{
	if (*app_p->app_timer == 0)
	{
		app_p->state = EXIT_SCREEN;
		App_KeyDisplay_ShowExitScreen(app_p, huart2_p);
	}
}

void App_KeyDisplay_ShowExitScreen(App_KeyDisplay *app_p, UART_HandleTypeDef *huart2_p)
{
	char *string1 = "Thank you!\n\r";
	char *string2 = "Have a nice day!\n\n\r";
	char *string3 = "Please reset (black) button to restart.\n\r";

	DisplayClear(huart2_p);
	DisplayHome(huart2_p);
	DisplayString(huart2_p, string1);
	DisplayString(huart2_p, string2);
	DisplayString(huart2_p, string3);
}

