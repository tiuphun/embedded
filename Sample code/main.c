#include <mcs51/8051.h>
#include "lcd1602.h"
#include<string.h>

#define LED           	P2_7
#define LED7SEG_DATA    P0

#define TEST_BUTTON P3_3
#define IR1_PIN  P3_2

int count;
int key_duration[4];
unsigned char key_count;

void main(void)
{
    count = 0;
    key_count = 0;
    LED7SEG_DATA = 0x00;

    TMOD = 0x02;    //timer 0 mode 1
    TH0 = 25;       //auto reload, period ~0.25ms
    TL0 = 0;

    IT1 = 1;
    IE  = 0b10000110;
    TF0 = 0;
    TR0 = 1;

    LED=1;

    char text[16] = "";
    int bitReceived=0;
    while(1)
    {
        if(!IR1_PIN){
                if (count>0) {
                    // start
                }
                if (count < 60 && count >= 30 ){
                    // after start
                    strcat(text, "#");
                }
                    if (count < 8 && count >= 0){
                    //0
                    strcat(text, "0");
                }
                    if (count >= 8 && count < 30){
                    //1
                    strcat(text, "1");
                }
            bitReceived++;
            count=0;
        }
        if (bitReceived >= 33){
            LCD_init();
            LCD_Write_String(text);
            LCD_Send_Command(0xC0); //Chuyen con tro xuong dong thu 2
            LCD_Write_String("****************");
            LCD_Send_Command(0xE); //Chuyen con tro xuong dong thu 2
            bitReceived = 0;
        }

    }
}

void TIMER0_ISR() __interrupt TF0_VECTOR
{
    count++;
}

void EX1_ISR() __interrupt IE1_VECTOR
{
    EX1 = 0;
    key_count++;
    if (key_count > 3)
        key_count = 3;
    key_duration[key_count] = count;
    count = 0;
    EX1 = 1;
}

