/*
 * App_KeyDisplay.h
 *
 *  Created on: Mar 9, 2025
 *      Author: peterh17
 */

#ifndef INC_APP_KEYDISPLAY_H_
#define INC_APP_KEYDISPLAY_H_

#ifdef _APP_KEYDISPLAY_C
   #define SCOPE
#else
   #define SCOPE extern
#endif


#define INTRO_SCREEN_WAIT 5000 	// 5 seconds
#define RESULT_SCREEN_WAIT 6000 // 6 seconds


enum _DisplayState
{
	INTRO_SCREEN, PROCESS_SCREEN, RESULT_SCREEN, EXIT_SCREEN
};
typedef enum _DisplayState DisplayState;


struct _App_KeyDisplay
{
	DisplayState state;

	//unsigned int Display_Timer;
	unsigned short *app_timer;

	// Counter to count how many keys are pushed
	unsigned short Numbers_Keys_Pushed;
	unsigned short Sum_of_Numbers_Pushed;
};
typedef struct _App_KeyDisplay App_KeyDisplay;

//-------------------------------------------------
//SCOPE unsigned short number_of_pushes;
//SCOPE unsigned int sum_of_pushed;
SCOPE unsigned short Star_first_press;


//---------------------------------------------------

SCOPE App_KeyDisplay App_KeyDisplay_construct(void);
SCOPE void App_KeyDisplay_ShowIntroScreen(UART_HandleTypeDef*);
SCOPE void App_KeyDisplay_loop(App_KeyDisplay*, HAL*, UART_HandleTypeDef*);
SCOPE void App_KeyDisplay_handleIntroScreen(App_KeyDisplay*, UART_HandleTypeDef*);
SCOPE void App_KeyDisplay_ShowProcessScreen(App_KeyDisplay*, UART_HandleTypeDef*);
SCOPE void App_KeyDisplay_ShowProcessScreen2(App_KeyDisplay*, HAL*, UART_HandleTypeDef*);
SCOPE void App_KeyDisplay_HandleProcessScreen(App_KeyDisplay*, HAL*, UART_HandleTypeDef*);
SCOPE void App_KeyDisplay_ShowResultScreen(App_KeyDisplay*, HAL*, UART_HandleTypeDef*);
SCOPE void App_KeyDisplay_HandleResultScreen(App_KeyDisplay*, HAL*, UART_HandleTypeDef*);
SCOPE void App_KeyDisplay_ShowExitScreen(App_KeyDisplay *app_p, UART_HandleTypeDef *huart2_p);
SCOPE void DisplayString(UART_HandleTypeDef*, char*);




#undef SCOPE
#endif /* INC_APP_KEYDISPLAY_H_ */
