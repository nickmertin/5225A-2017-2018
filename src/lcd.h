/* Defines */
// The start point of the LCD
#ifndef LCD_START_POINT
#define LCD_START_POINT 0
#endif

// Check the state of LCD buttons
#define LCD_HIGH(btn) CHK_BIT(btn, _LCDButtons)
#define LCD_LOW(btn) !CHK_BIT(btn, _LCDButtons)
#define LCD_RISING(btn) (CHK_BIT(btn, _LCDButtons) && !CHK_BIT(btn, _LCDButtonsLst))
#define LCD_FALLING(btn) (!CHK_BIT(btn, _LCDButtons) && CHK_BIT(btn, _LCDButtonsLst))

/* Variables */
unsigned long gLCDCurPosition = LCD_START_POINT; // The current LCD position
TControllerButtons _LCDButtons = nLCDButtons; // The currnet state of the LCD buttons
TControllerButtons _LCDButtonsLst = _LCDButtons; // The last state of the LCD buttons

/* Functions */
void handleLCD(); // Handle the LCD
void clearLCD(); // Clear the LCD
void extendNumber(unsigned long number, int length, char c, char* output); // Pad a number with a character
void extendNumber(unsigned long number, int length, char* output); // Pad a number with spaces
