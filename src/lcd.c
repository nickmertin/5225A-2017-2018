#ifndef LCD_MENU_ITEMS
#error "LCD_MENU_ITEMS not defined"
#endif

void handleLCD()
{
	_LCDButtonsLst = _LCDButtons;
	_LCDButtons = nLCDButtons;

	// Check the left and right button presses and scroll the correct way
	if (LCD_RISING(kButtonLeft))
	{
		if (gLCDCurPosition == LCD_START_POINT) gLCDCurPosition = LCD_MENU_ITEMS + LCD_START_POINT - 1;
		else gLCDCurPosition -= 1;
	}
	if (LCD_RISING(kButtonRight))
	{
		if (gLCDCurPosition == LCD_MENU_ITEMS - 1) gLCDCurPosition = LCD_START_POINT;
		else gLCDCurPosition += 1;
	}
}

void clearLCD()
{
	clearLCDLine(0);
	clearLCDLine(1);
}

void extendNumber(unsigned long number, int length, char c, char* output)
{
	int len = number ? ((long)log10(abs(number))) + 1 : 1;
	if (number < 0) len++;

	sprintf(output, "%d", number);
	memset(output + len, c, length - len);
	memset(output + length, '\0', 1);
}

void extendNumber(unsigned long number, int length, char* output) { extendNumber(number, length, ' ', output); }
