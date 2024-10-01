#include <mcs51/8051.h>

#define LED           	P2_7
#define LED7SEG_DATA    P0
#define BUTTON          P3_3

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

    while(1)
    {
		if (key_count >= 2 && key_duration[key_count] <= 800) {
            LED = 0;
            key_count = 0;
        }
        else if (count >= 2000) {
            LED = 1;
            key_count = 0;
            count = 0;
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
