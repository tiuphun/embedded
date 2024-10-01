/* Sample: Interrupt handling */

#include <mcs51/8052.h>

#define LED_1    P2_0
#define EX0_PIN  P3_2
void ISR0(void) __interrupt IE0_VECTOR;

void main(void)
{
    //initialize interrupt
    IT0 = 1;

    EX0 = 1;
    EA  = 1;
    EX0_PIN = 1;

    while(1);

}

void ISR0() __interrupt IE0_VECTOR
{
    LED_1 = !LED_1;
}
