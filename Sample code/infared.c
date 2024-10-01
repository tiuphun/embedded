#include <mcs51/8051.h>

#define LED           	P2_7
#define LED7SEG_DATA    P0
#define BUTTON          P3_3
#define IR1             P3_2

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

    char cmd1[20];
    char cmd2[20];
    while(1)
    {
		if (count > 5000) {
            P2 = 0xff;
            count = 0;
        }

        if (key_count >= 1) {
            key_count = 0;
            P2_1 = 0;
            P2_7 = 0;
            int j = 0;
            for (int i = 0; i < 16; i++) {
                while (key_count < 2);
                dur = key_duration[2];
                key_count = 1;
                if (dur < 7) {
                    cmd1[j] = '0';
                } else if (dur < 30) {
                    cmd1[j] = '1';
                }
                j++;
            }
            cmd1[j] = '\0';

            j = 0;
            for (int i = 16; i < 32; i++) {
                while (key_count < 2);
                dur = key_duration[2];
                key_count = 1;
                if (dur < 7) {
                    cmd2[j] = '0';
                } else if (dur < 30) {
                    cmd2[j] = '1';
                }
                j++;
            }
            cmd2[j] = '\0';

            LCD_init();
            LCD_Write_String(cmd1);
            LCD_Send_Command(0xC0);
            LCD_Write_String(cmd2);
            LCD_Send_Command(0xE);   
        }
        key_count = 0;
        count = 0;
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
