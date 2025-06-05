/** uart.c
 * Implements higher level UART send and receive with callbacks
 *
 * Author: Alex Castellar
*/

#define _UART_C

#include "uart.h"
#include "main.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static UART_HandleTypeDef *huart = NULL;
static char recvChar;
static volatile int recved;

/* setup function */
void UART_setup(UART_HandleTypeDef *huartRef) {
	huart = huartRef;
	recved = 0;
	recvChar = '\0';
	HAL_UART_Receive_IT(huart, (uint8_t *) &recvChar, 1);
}

/* recv functions */
void UART_irq() {
	recved = 1;
}

void UART_process(void (*uartCallback)(char c)) {
	if (recved) {
		uartCallback(recvChar);
		recved = 0;

		// restart callback
		HAL_UART_Receive_IT(huart, (uint8_t *) &recvChar, 1);
	}
}

/* send functions */
void UART_send(char *str) {
	HAL_UART_Transmit(huart, (uint8_t *) str, strlen(str), HAL_MAX_DELAY);
}

void UART_send_char(char c) {
	HAL_UART_Transmit(huart, (uint8_t *) &c, 1, HAL_MAX_DELAY);
}

void UART_send_newline() {
	UART_send("\r\n");
}

void UART_send_clear_screen() {
    char ClearScreen[] = { 0x1B, '[', '2' , 'J', 0 };
    UART_send(ClearScreen);
}

void UART_send_cursor_pos(int row, int col)
{
    static char CurPos[20];

    sprintf(CurPos, "\x1B[%d;%dH", row, col);
    UART_send(CurPos);
}

