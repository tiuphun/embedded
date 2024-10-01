#include <mcs51/8051.h>

#define TH0_50ms        0x4C
#define TL0_50ms        0x00
#define LED_1           P2_0
#define LED7SEG_DATA    P0

int count;

void main(void)
{
    count = 0;
    LED7SEG_DATA = 0x00;

    TMOD = 0x01;    //timer 0 mode 1
    TH0 = TH0_50ms;
    TL0 = TL0_50ms;

    IE  = 0b10000010;
    TF0 = 0;
    TR0 = 1;

    while(1);
}

void TIMER0_ISR() __interrupt TF0_VECTOR
{
    TH0 = TH0_50ms;
    TL0 = TL0_50ms;

    count++;
    if (count == 20)
    {
        count = 0;
        LED_1 = !LED_1;
    }
}
