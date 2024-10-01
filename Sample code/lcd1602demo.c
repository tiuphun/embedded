#include <mcs51/8052.h>
#include "lcd1602.h"
#include "string.h"

void main()
{
	int i;
	LCD_init();
	LCD_Write_String("Hello Elitech 65");
	LCD_Send_Command(0xC0); //Chuyen con tro xuong dong thu 2
	LCD_Write_String("****************");
	LCD_Send_Command(0xE); //Chuyen con tro xuong dong thu 2
	while(1){
		//--------------
		//your code here
		//---------------
	}
}



