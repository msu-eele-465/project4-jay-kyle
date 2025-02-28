/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430FR235x Demo - Toggle P1.0 using software
//
//  Description: Toggle P1.0 every 0.1s using software.
//  By default, FR235x select XT1 as FLL reference.
//  If XT1 is present, the PxSEL(XIN & XOUT) needs to configure.
//  If XT1 is absent, switch to select REFO as FLL reference automatically.
//  XT1 is considered to be absent in this example.
//  ACLK = default REFO ~32768Hz, MCLK = SMCLK = default DCODIV ~1MHz.
//
//           MSP430FR2355
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |           P1.0|-->LED
//
//   Cash Hao
//   Texas Instruments Inc.
//   November 2016
//   Built with IAR Embedded Workbench v6.50.0 & Code Composer Studio v6.2.0
//******************************************************************************
#include <msp430.h>

char pressed_key = 'N';                     // holds key that was pressed
int password_index = 1;                     // tracks which digit is being entered
int status = 0;                             // tracks locked, unlocked, or unlocking
int locked = 0;                             // status value for locked
int unlocking = 1;                          // status value for unlocking
int unlocked = 2;                           // status value for unlocked
char password_char1 = '5';                  // first digit of password
char password_char2 = '2';                  // second digit of password
char password_char3 = '9';                  // third digit of password
char password_char4 = '3';                  // fourth digit of password

void keypad_init(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer           

    P1DIR |= (BIT4 | BIT5 | BIT6 | BIT7);   // Set keypad row pins as outputs
    P1OUT |= (BIT4 | BIT5 | BIT6 | BIT7);   // sets output high to start

    P2DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3);  // Set P1.4 - p1.7 as input
    P2REN |= (BIT0 | BIT1 | BIT2 | BIT3);   // Enable pull-up/down resistors
    P2OUT &= ~(BIT0 | BIT1 | BIT2 | BIT3);  // Set as pull-down
    P2IES |= (BIT0 | BIT1 | BIT2 | BIT3);   // Config IRQ sensitivity H-to-L

    PM5CTL0 &= ~LOCKLPM5;                   // Disable low power mode

    P2IFG &= ~(BIT0 | BIT1 | BIT2 | BIT3);  // Clear P2.0-2.3 IRQ Flag
    P2IE |= (BIT0 | BIT1 | BIT2 | BIT3);    // Enable P2.0-2.3 IRQ
    __enable_interrupt();                   // Enable Maskable IRQs
}

int main(void)
{
    keypad_init();

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    
    P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;                          // Set P1.0 to output direction

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

    while(1)
    {
        P1OUT ^= BIT0;                      // Toggle P1.0 using exclusive-OR
        __delay_cycles(100000);             // Delay for 100000*(1/MCLK)=0.1s
    }
}

void get_key(void) {
    char key_map[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    for (int row = 0; row < 4; row++) {
        P1OUT &= ~(BIT4 | BIT5 | BIT6 | BIT7);  // Clear all rows
        P1OUT |= (BIT4 << row);  // Set one row high at a time
        __delay_cycles(1000); 

        for (int col = 0; col < 4; col++) {
            if (P2IN & (BIT0 << col)) {  // Check if column is high
                process_key(key_map[row][col]);
            }
        }
    }
    P1OUT |= (BIT4 | BIT5 | BIT6 | BIT7);  // Reset all rows
    return 'N';  // No key pressed
}

void process_key(char pressed_key) {
    if (status == locked || status == unlocking) {
        check_password(pressed_key);
    } else {
        // send data to slave with I2C
    }
}

void check_password(char pressed_key) {
    if (password_index == 1) {
        if (pressed_key == password_char1) {
            status = unlocking;
            password_index = 2;
        } else {
            status = locked;
            password_index = 1;
        }
    } else if (password_index == 2) {
        if (pressed_key == password_char2) {
            status = unlocking;
            password_index = 3;
        } else {
            status = locked;
            password_index = 1;
        }
    } else if (password_index == 3) {
        if (pressed_key == password_char3) {
            status = unlocking;
            password_index = 4;
        } else {
            status = locked;
            password_index = 1;
        }
    } else if (password_index == 4) {
        if (pressed_key == password_char4) {
            status = unlocked;
            password_index = 1;
        } else {
            status = locked;
            password_index = 1;
        }
    }
}

#pragma vector=PORT2_VECTOR
__interrupt void KEYPAD_ISR(void) {
    get_key();
    P2IFG &= ~(BIT0 | BIT1 | BIT2 | BIT3);
}
