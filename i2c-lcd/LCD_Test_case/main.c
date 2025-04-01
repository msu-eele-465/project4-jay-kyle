/*#include <msp430.h> 

    int pattern = 0;
    int cursor_status = 1;
    int blink_status = 1;
    int i = 2;
    int j = 0;
    int k = 0;
    int timing_step = 0;

    TB0CTL |= TBCLR;
    TB0CTL |= TBSSEL__ACLK;
    TB0CTL |= MC__UP;
    TB0CCR0 = 16384;

    TB0CCTL0 |= CCIE;
    TB0CCTL0 &= ~CCIFG; */
/**
 * main.c
 
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    //-- Setup Ports

    P6DIR   |= 0b1111111;
    P6OUT   &= ~0b1111111;
    PM5CTL0 &= ~LOCKLPM5;


    setup();




    while(1){


        clear_display();
        __delay_cycles(4000);
        return_home();
        __delay_cycles(4000);
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
        __delay_cycles(4000);
        for(k = 0; k < 16; k = k+1){
            //for(j = 0; j > 13, j++){
                    byte(i, j);



        }
        P6OUT   &= ~BIT6;
        __delay_cycles(4000);
            byte(0b1100, 0b0000);
            // End DD Ram set key-------------------------------------------

            // write Key
            P6OUT   |= BIT6;
            P6OUT   &= ~BIT5;

        for(k = 0; k < 16; k = k+1){
            //for(j = 0; j > 13, j++){
                    byte(i, j);
        }

        j = j+1;
        if(j > 15){
            j = 0;
            i = i+1;
        }

        if(i == 8){
            i = 10;
            j = 1;
        }
        if(i == 16){
            i = 2;
        }


        __delay_cycles(150000);


    }

    return 0;
}



int byte(int upper, int lower){
    //timing_step = 1;
    //TB0CCR0 = 1;
    __delay_cycles(3000);
    P6OUT   &= ~0b1111;
    P6OUT   |= upper;
    P6OUT   |= BIT4;
    __delay_cycles(3000);
    P6OUT   &= ~BIT4;
    __delay_cycles(3000);

    __delay_cycles(3000);
    P6OUT   &= ~0b1111;
    P6OUT   |= lower;
    P6OUT   |= BIT4;
    __delay_cycles(3000);
    P6OUT   &= ~BIT4;
    __delay_cycles(3000);
    // End DD Ram set key-------------------------------------------
    return 0;
}

int clear_display(void){
    P6OUT   &= ~BIT6;
    P6OUT   &= ~BIT5;
    // Clear display
    byte(0b0000, 0b0001);

    // End Clear display -------------------------------------------
}

int return_home(void){
    P6OUT   &= ~BIT6;
    P6OUT   &= ~BIT5;
    // Return Home -------------------------------------------
    byte(0b0000, 0b0010);

    // End Return Home -------------------------------------------
}
int setup(void){
    // 4 Bit Mode -----------------------------------------
    P6OUT   &= ~BIT6;
    P6OUT   &= ~BIT5;

    byte(0b0010, 0b1100);

    //  End 4 Bit Mode -----------------------------------------

    clear_display();

    return_home();

    // Display on -------------------------------------------
    byte(0b0000, 0b1111);

    //End Display on  -------------------------------------------
    return 0;
}

/ ---------- ISR ------------------
#pragma vector = TIMER0_B0_VECTOR
__interrupt void ISR_TB0_CCR0(void){
    if(timing_step == 0){
        return;
    }
    else if(timing_step == 1){
        P6OUT   &= ~0b1111;
        P6OUT   |= upper;
        P6OUT   |= BIT4;
    }


    TB0CCTL0 &= ~CCIFG;
}*/
