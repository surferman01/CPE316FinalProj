#define _KEYPAD_C

#include <stdbool.h>
#include "keypad.h"
#include "timer.h"
#include "main.h"
#include "shotclock.h"
#include "uart.h"
#include "shiftreg.h"

void KeypadClear() {
	int sIndex;

	// Clear all debounced records, Previous, Low2High
	for (sIndex=0; sIndex<Number_of_Cols; sIndex++)
	{
		sKeyDebouncedCol[sIndex] = 0x0000;
		sKeyPreviousCol[sIndex] = 0x0000;
		sKeyLow2HighCol[sIndex] = 0x0000;
		sKeyHigh2LowCol[sIndex] = 0x0000;
	}
}

void KeypadScan()
{
	unsigned short sIndex;
	unsigned short Temp;

	// Clear all key records
	for (sIndex=0; sIndex<Number_of_Cols; sIndex++)
	{
		sKeyCurrentCol[sIndex] = 0x00;
	}

	// Read all 3 column
	for (sIndex=0; sIndex<Number_of_Keys; sIndex++)
	{
		GPIOA->ODR &=~(PA4 | PA1 | PA0);
		GPIOA->ODR |= sKeyControl[sIndex].sKeySend;

		switch (sKeyControl[sIndex].KeyLetter)
		{
		case '1':
		case '2':
		case '3':
			if (GPIOA->IDR & sKeyControl[sIndex].sKeyRead)
				sKeyCurrentCol[sKeyControl[sIndex].sKeyCol]= sKeyControl[sIndex].sKeyReadTempPos;
			break;

		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if (GPIOC->IDR & sKeyControl[sIndex].sKeyRead)
				sKeyCurrentCol[sKeyControl[sIndex].sKeyCol] = sKeyControl[sIndex].sKeyReadTempPos;
			break;

		case '*':
		case '0':
		case '#':
			if (GPIOB->IDR & sKeyControl[sIndex].sKeyRead)
				sKeyCurrentCol[sKeyControl[sIndex].sKeyCol] = sKeyControl[sIndex].sKeyReadTempPos;
		}
	}

	// Check if a key is steadily read
	for (sIndex=0; sIndex<Number_of_Cols; sIndex++)
	{
		if ((sKeyCurrentCol[sIndex] == sKeyDebouncedCol[sIndex]) && (sKeyCurrentCol[sIndex] != 0x0000))
		{
			sKeyStatus |= KeyIsOn;
			break;
		}

		if ((sKeyCurrentCol[sIndex] == sKeyDebouncedCol[sIndex]) && (sKeyCurrentCol[sIndex] == 0x0000) && (sKeyPreviousCol[sIndex]!= 0x0000))
		{
			sKeyStatus |= KeyIsOff;
			break;
		}

	}

	if (sIndex <Number_of_Cols)
	{
		for (sIndex=0; sIndex<Number_of_Cols; sIndex++)
		{
			Temp = sKeyCurrentCol[sIndex] ^ sKeyPreviousCol[sIndex];
			sKeyLow2HighCol[sIndex] = (sKeyCurrentCol[sIndex] & Temp);
			sKeyHigh2LowCol[sIndex] = (sKeyPreviousCol[sIndex] & Temp);

		}

		// Find which key is JUST depressed (Low To High) or KeyRepeat detected
		for (sIndex=0 ; sIndex<Number_of_Keys; sIndex++)
		{
			if (sKeyStatus & KeyIsOn)
			{
				if (sKeyLow2HighCol[sKeyControl[sIndex].sKeyCol] & sKeyControl[sIndex].sKeyReadTempPos)
				{
					sKeyIssued = sKeyControl[sIndex].KeyLetter;
					sKeyStatus |= (KeyDetect | KeyLow2High);
					sTimer[KEY_WAIT_REPEAT_TIMER] = KEY_WAIT_REPEAT_TIME;
					sKeyStatus |= KeyRepeat;		// a new key comes in, set the repeat flag
					sIndexCopy = sIndex;			// save a copy of sIndex for push & held use
					break;
				}
				else if ((sKeyStatus & KeyRepeat) && (sTimer[KEY_WAIT_REPEAT_TIMER]==0))
				{
					if (sTimer[KEY_REPEAT_TIMER] == 0)
					{
						sKeyIssued = sKeyControl[sIndexCopy].KeyLetter;
						sKeyStatus |= (KeyDetect | KeyToBeRepeated);
						sTimer[KEY_REPEAT_TIMER] = KEY_REPEAT_TIME;
					}
				}
				else
					sKeyIssued = 0xFFFF;
			}
			else if (sKeyStatus & KeyIsOff)
			{
				if (sKeyHigh2LowCol[sKeyControl[sIndex].sKeyCol] & sKeyControl[sIndex].sKeyReadTempPos)
				{
					sKeyReleased = sKeyControl[sIndex].KeyLetter;
					sKeyStatus |= (KeyDetect | KeyHigh2Low);
					break;
				}
				else
					sKeyReleased = 0xFFFF;
			}

		}
	}
	else
	{
		sKeyStatus &= ~(KeyDetect | KeyLow2High | KeyHigh2Low | KeyIsOn | KeyIsOff | KeyToBeRepeated | KeyRepeat);
		sTimer[KEY_REPEAT_TIMER] = 0;  // Reset repeat timer if no key

	}

	for (sIndex=0; sIndex<Number_of_Cols; sIndex++)
	{
		if (sKeyCurrentCol[sIndex] == sKeyDebouncedCol[sIndex])
		{
			// Update Previous records
			sKeyPreviousCol[sIndex] = sKeyCurrentCol[sIndex];
		}
	}


	// Transfer Current reading to debounced reading
	for (sIndex=0; sIndex<Number_of_Cols; sIndex++)
	{
		sKeyDebouncedCol[sIndex] = sKeyCurrentCol[sIndex];
		sKeyLow2HighCol[sIndex] = 0;
		sKeyHigh2LowCol[sIndex] = 0;
	}
}


void KeyProcess(void (*keyPressedCallback)(char key), void (*keyRepeatedCallback)(char key), void (*keyReleasedCallback)(char key))
{
	uint16_t sIndex;

	if ((sKeyStatus & KeyDetect) && (sKeyIssued != 0xFF))
	{

		if (keyPressedCallback && sKeyStatus & KeyLow2High) {
			keyPressedCallback(sKeyIssued);
		} else if (keyRepeatedCallback && sKeyStatus & KeyToBeRepeated) {
			keyRepeatedCallback(sKeyIssued);
		} else if (keyReleasedCallback && sKeyStatus & KeyHigh2Low) {
			keyReleasedCallback(sKeyIssued);
		}


		sKeyStatus &= ~(KeyDetect | KeyLow2High | KeyToBeRepeated);

		// Clear all Low-2-High and High-2-Low records
		for (sIndex=0; sIndex<Number_of_Cols; sIndex++)
			sKeyLow2HighCol[sIndex] = 0x0000;
	}
}

// i want the following feature. if you press a button,
// it reads the next 6 buttons you press and the order you
// press the buttons puts the value at that location

int index_store;
bool CHOOSE_VAL = false;

void key1() {
	if (CHOOSE_VAL == true) {
		// set the shift reg data to be the hex value for the number pressed
		SR_Data[index_store] = SSEG_TT[1];
		CHOOSE_VAL = false;
		return;
	}
	// here would be if its the first time the button is pressed
	UART_send("NOW CHOOSE VALUE FOR SSEG0:\n\r");
	index_store = SSEG0;
	CHOOSE_VAL = true;

}
void key2() {
	if (CHOOSE_VAL == true) {
		// set the shift reg data to be the hex value for the number pressed
		SR_Data[index_store] = SSEG_TT[2];
		CHOOSE_VAL = false;
		return;
	}
	UART_send("NOW CHOOSE VALUE FOR SSEG1:\n\r");
	index_store = SSEG1;
	CHOOSE_VAL = true;
}
void key3() {
	if (CHOOSE_VAL == true) {
		// set the shift reg data to be the hex value for the number pressed
		SR_Data[index_store] = SSEG_TT[3];
		CHOOSE_VAL = false;
		return;
	}
	UART_send("NOW CHOOSE VALUE FOR SSEG2:\n\r");
	index_store = SSEG2;
	CHOOSE_VAL = true;
}
void key4() {
	if (CHOOSE_VAL == true) {
		// set the shift reg data to be the hex value for the number pressed
		SR_Data[index_store] = SSEG_TT[4];
		CHOOSE_VAL = false;
		return;
	}
	UART_send("NOW CHOOSE VALUE FOR SSEG3:\n\r");
	index_store = SSEG3;
	CHOOSE_VAL = true;
}
void key5() {
	if (CHOOSE_VAL == true) {
		// set the shift reg data to be the hex value for the number pressed
		SR_Data[index_store] = SSEG_TT[5];
		CHOOSE_VAL = false;
		return;
	}
	UART_send("NOW CHOOSE VALUE FOR SSEG4:\n\r");
	index_store = SSEG4;
	CHOOSE_VAL = true;
}
void key6() {
	if (CHOOSE_VAL == true) {
		// set the shift reg data to be the hex value for the number pressed
		SR_Data[index_store] = SSEG_TT[6];
		CHOOSE_VAL = false;
		return;
	}
	UART_send("NOW CHOOSE VALUE FOR SSEG5:\n\r");
	index_store = SSEG5;
	CHOOSE_VAL = true;

}
void key7() {
	if (CHOOSE_VAL == true) {
		// set the shift reg data to be the hex value for the number pressed
		SR_Data[index_store] = SSEG_TT[7];
		CHOOSE_VAL = false;

		return;
	}

}
void key8() {
	if (CHOOSE_VAL == true) {
		// set the shift reg data to be the hex value for the number pressed
		SR_Data[index_store] = SSEG_TT[8];
		CHOOSE_VAL = false;
		return;
	}

}
void key9() {
	if (CHOOSE_VAL == true) {
		// set the shift reg data to be the hex value for the number pressed
		SR_Data[index_store] = SSEG_TT[9];
		CHOOSE_VAL = false;
		return;
	}

}
void key0() {
	if (CHOOSE_VAL == true) {
		// set the shift reg data to be the hex value for the number pressed
		SR_Data[index_store] = SSEG_TT[0];
		CHOOSE_VAL = false;
		return;
	}
	horn();
}
void keyS() {
	if (CHOOSE_VAL == true) {
		UART_send("INVALID CHOICE\n\rRESET");
		CHOOSE_VAL = false;
		return;
	}
	ShiftReg_output_enable();
}
void keyP() {
	if (CHOOSE_VAL == true) {
		UART_send("INVALID CHOICE\n\rRESET");
		CHOOSE_VAL = false;
		return;
	}
	ShiftReg_output_disable();
}
