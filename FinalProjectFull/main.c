#include "msp.h"
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define OFF 0
#define MODE1 1
#define MODE_2 2
#define MODE_3 3

#define BUTTON_THRESHOLD 10000
int buttonState = BUTTON_THRESHOLD;

void setGates(int g1, int g2) {
    if(g1)
        P6OUT |= BIT6;
    else
        P6OUT &= !BIT6;
    if(g2)
        P6OUT |= BIT7;
    else
        P6OUT &= !BIT7;
}

/**
 * main.c
 */
    void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	    initUART();

	    clear_display();

	    time_t t;

	    int mode = OFF;

	        // mode button
	        P1DIR &= !BIT5;

	        // gate controls
	        P6DIR |= BIT6 | BIT7;
	        P6OUT &= !(BIT6 | BIT7);

	        int delayTime = 0, delay = 200000;

	        while(1)
	        {
	            delayTime++;

	            if(delayTime > delay) {
	                delayTime = 0;

	                time(&t);
	                char *t_str[] = ctime(&t);
	                clear_display();
	                uart_print(*t_str);
	            }

	            if(buttonState < BUTTON_THRESHOLD)
	                buttonState++;

	            if(!(P1IN & BIT5))
	                buttonState = 0;

	            switch(mode) {

	            case OFF:
	                if(buttonState == BUTTON_THRESHOLD - 1)
	                    mode = MODE1;
	                setGates(0, 0);
	                break;
	            case MODE1:
	                if(buttonState == BUTTON_THRESHOLD - 1)
	                    mode = MODE_2;
	                setGates(1, 0);
	                break;
	            case MODE_2:
	                if(buttonState == BUTTON_THRESHOLD - 1)
	                    mode = OFF;
	                setGates(0, 1);
	                break;
	            }
	        }
}
