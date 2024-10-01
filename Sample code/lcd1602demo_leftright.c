#include <mcs51/8052.h>
#include "lcd1602.h"
#include "string.h"

void main()
{
    /*
    LCD_Send_Command(0x38); //Chon che do 8 bit, 2 hang cho LCD
	LCD_Send_Command(0x0E); //Bat hien thi, nhap nhay con tro
	LCD_Send_Command(0x01); //Xoa man hinh
	LCD_Send_Command(0x80); //Ve dau dong
    */

	char hello[20] = "Hello Elitech 66";
	char star[20]  = "****************";

	char n_hello[20];
	char n_star[20];
	while(1){
		for(int i=0; i<=strlen(hello); i++){
            int j=0;
            for(int k=i; k<strlen(hello); k++){
                n_hello[j] = hello[k];
                j++;
            }
            n_hello[j] = '\0';

            j=0;
            for(int k=i; k<strlen(star); k++){
                n_star[j] = star[k];
                j++;
            }
            n_star[j] = '\0';

            LCD_init();
            LCD_Write_String(n_hello);
            LCD_Send_Command(0xC0); //Chuyen con tro xuong dong thu 2
            LCD_Write_String(n_star);
            LCD_Send_Command(0xE); //Chuyen con tro xuong dong thu 2
            Delay_ms(10);
		}

		for(int i=strlen(hello)-1; i>=0; i--){
            int j=0;
            for(int k=i; k<strlen(hello); k++){
                n_hello[j] = hello[k];
                j++;
            }
            n_hello[j] = '\0';

            j=0;
            for(int k=i; k<strlen(star); k++){
                n_star[j] = star[k];
                j++;
            }
            n_star[j] = '\0';

            LCD_init();
            LCD_Write_String(n_hello);
            LCD_Send_Command(0xC0); //Chuyen con tro xuong dong thu 2
            LCD_Write_String(n_star);
            LCD_Send_Command(0xE); //Chuyen con tro xuong dong thu 2
            Delay_ms(10);
		}
		Delay_ms(100);
	}
}



