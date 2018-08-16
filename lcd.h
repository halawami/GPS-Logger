/*
 *	LCD interface header file
 *	See lcd.c for more info
 */

extern void Display( unsigned int );

/* delay for indicated number of milliseconds */
extern void DelayMs(unsigned int);

extern void LCD_strobe();

/* write a byte to the LCD in 8 bit mode */
extern void lcd_write(unsigned char, unsigned char);

/* Clear and home the LCD */
extern void lcd_clear(void);

/* write a string of characters to the LCD */
extern void lcdWriteString(const char * s, char);

/* Go to the specified position */
extern void lcd_goto(unsigned char pos);
	
/* intialize the LCD - call before anything else */
extern void initLCD(void);

extern void lcd_putch(char);

/*	Set the cursor position */
#define	lcd_cursor(x)	lcd_write(((x)&0x7F)|0x80)
