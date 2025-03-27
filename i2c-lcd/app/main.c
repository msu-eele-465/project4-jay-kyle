/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
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
//  MSP430FR231x Demo - Toggle P1.0 using software
//
//  Description: Toggle P1.0 every 0.1s using software.
//  By default, FR231x select XT1 as FLL reference.
//  If XT1 is present, the PxSEL(XIN & XOUT) needs to configure.
//  If XT1 is absent, switch to select REFO as FLL reference automatically.
//  XT1 is considered to be absent in this example.
//  ACLK = default REFO ~32768Hz, MCLK = SMCLK = default DCODIV ~1MHz.
//
//           MSP430FR231x
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |           P1.0|-->LED
//
//   Darren Lu
//   Texas Instruments Inc.
//   July 2015
//   Built with IAR Embedded Workbench v6.30 & Code Composer Studio v6.1 
//******************************************************************************
#include <msp430.h> 

int pattern = 0;
int cursor_status = 1;
int blink_status = 1;

int Data_Cnt = 0;
int Data_In[] = {0x00, 0x00, 0x00};

int status = -1;
int key_num = -1;
float base_transition_period = 0.00;

void i2c_b0_init(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    UCB0CTLW0 |= UCSWRST;                   // Put eUSCI_B0 in SW Reset
    UCB0CTLW0 |= UCMODE_3;                  // Put into I2C mode
    UCB0CTLW0 |= UCSYNC;                    // Synchronous
    UCB0CTLW0 &= ~UCMST;                    // Put into slave mode
    UCB0I2COA0 = 0x0020 | UCOAEN;           // Own address + enable bit
    UCB0CTLW1 &= ~UCASTP_3;                 // Use manual stop detection

    P1SEL1 &= ~BIT3;                        // P1.3 = SCL
    P1SEL0 |= BIT3;                            
    P1SEL1 &= ~BIT2;                        // P1.2 = SDA
    P1SEL0 |= BIT2;

    PM5CTL0 &= ~LOCKLPM5;                   // Disable low power mode

    UCB0CTLW0 &= ~UCSWRST;                  // Take eUSCI_B0 out of SW Reset

    UCB0IE |= UCRXIE0 | UCSTPIE | UCSTTIE;  // Enable I2C Rx0 IR1
    __enable_interrupt();                   // Enable Maskable IRQs
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       // Disable high z mode

	//-- Setup Ports
	P1DIR   |= 0b11110011;
    P1OUT   &= ~0b11110011;
    P2DIR   |= BIT0;
    P2OUT   &= ~BIT0;

	setup();
    i2c_b0_init();

	while(1){
	    clear_display();
        __delay_cycles(40000);
	    print_pattern(7);
        __delay_cycles(40000);
	    print_key('A');
        __delay_cycles(40000);
	    print_key('B');
        __delay_cycles(40000);
	    print_key('C');
        __delay_cycles(40000);
	    print_key('D');
        __delay_cycles(40000);
	    print_key('9');
	    print_key('1');
	    print_key('C');
	    print_key('9');
	    print_key('C');
	    print_key('5');
	    print_key('6');
	    print_key('7');
	    print_key('9');
	}
	return 0;
}

void process_i2c_data(void) {
    status = Data_In[0];
    key_num = Data_In[1];
    base_transition_period = Data_In[2] * 0.25;
}

int print_pattern(int pattern){
    if(pattern == 0){
        return_home();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0111,0b0011);
        byte(0b0111,0b0100);
        byte(0b0110,0b0001);
        byte(0b0111,0b0100);
        byte(0b0110,0b1001);
        byte(0b0110,0b0011);
    }
    else if(pattern == 1){
        return_home();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0111,0b0100);
        byte(0b0110,0b1111);
        byte(0b0110,0b0111);
        byte(0b0110,0b0111);
        byte(0b0110,0b1100);
        byte(0b0110,0b0101);
    }
    else if(pattern == 2){
        return_home();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0111,0b0101);
        byte(0b0111,0b0000);
        shift_right();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0110,0b0011);
        byte(0b0110,0b1111);
        byte(0b0111,0b0101);
        byte(0b0110,0b1110);
        byte(0b0111,0b0100);
        byte(0b0110,0b0101);
        byte(0b0111,0b0010);
    }
    else if(pattern == 3){
        return_home();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0110,0b1001);
        byte(0b0110,0b1110);
        shift_right();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0110,0b0001);
        byte(0b0110,0b1110);
        byte(0b0110,0b0100);
        shift_right();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0110,0b1111);
        byte(0b0111,0b0101);
        byte(0b0111,0b0100);

    }
    else if(pattern == 4){
        return_home();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0110,0b0100);
        byte(0b0110,0b1111);
        byte(0b0111,0b0111);
        byte(0b0110,0b1110);
        shift_right();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0110,0b0011);
        byte(0b0110,0b1111);
        byte(0b0111,0b0101);
        byte(0b0110,0b1110);
        byte(0b0111,0b0100);
        byte(0b0110,0b0101);
        byte(0b0111,0b0010);
    }
    else if(pattern == 5){
        return_home();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0111,0b0010);
        byte(0b0110,0b1111);
        byte(0b0111,0b0100);
        byte(0b0110,0b0001);
        byte(0b0111,0b0100);
        byte(0b0110,0b0101);
        shift_right();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0011,0b0001);
        shift_right();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0110,0b1100);
        byte(0b0110,0b0101);
        byte(0b0110,0b0110);
        byte(0b0111,0b0100);
    }
    else if(pattern == 6){
        return_home();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0111,0b0010);
        byte(0b0110,0b1111);
        byte(0b0111,0b0100);
        byte(0b0110,0b0001);
        byte(0b0111,0b0100);
        byte(0b0110,0b0101);
        shift_right();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0011,0b0111);
        shift_right();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0111,0b0010);
        byte(0b0110,0b1001);
        byte(0b0110,0b0111);
        byte(0b0110,0b1000);
        byte(0b0111,0b0100);
    }
    else if(pattern == 7){
        return_home();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0110,0b0110);
        byte(0b0110,0b1001);
        byte(0b0110,0b1100);
        byte(0b0110,0b1100);
        shift_right();
        P1OUT   |= BIT0;
        P1OUT   &= ~BIT1;
        byte(0b0110,0b1100);
        byte(0b0110,0b0101);
        byte(0b0110,0b0110);
        byte(0b0111,0b0100);
    }
    return 0;
}

int print_key(int key){
    // DD Ram set key-------------------------------------------
    P1OUT   &= ~BIT0;
    byte(0b1100, 0b1111);
    // End DD Ram set key-------------------------------------------

    // write Key
    P1OUT   |= BIT0;
    P1OUT   &= ~BIT1;
    if(key == 'A'){byte(0b0100, 0b0001);}

    else if(key == 'B'){byte(0b0100, 0b0010);}

    else if(key == 'C'){
        byte(0b0100, 0b0011);
        P1OUT   &= ~BIT0;
        cursor_status ^=  0b1;
        if (blink_status == 1){
            if(cursor_status == 1){
                byte(0b0000, 0b1111);
            }
            else{
                byte(0b0000, 0b1101);
            }
        }
        else{
            if(cursor_status == 1){
                byte(0b0000, 0b1110);
            }
            else{
                byte(0b0000, 0b1100);
            }
        }
    }

    else if(key == 'D'){byte(0b0100, 0b0100);}

    else if(key == '0'){byte(0b0011, 0b0000);}

    else if(key == '1'){byte(0b0011, 0b0001);}

    else if(key == '2'){byte(0b0011, 0b0010);}

    else if(key == '3'){byte(0b0011, 0b0011);}

    else if(key == '4'){byte(0b0011, 0b0100);}

    else if(key == '5'){byte(0b0011, 0b0101);}

    else if(key == '6'){byte(0b0011, 0b0110);}

    else if(key == '7'){byte(0b0011, 0b0111);}

    else if(key == '9'){
        byte(0b0011, 0b1001);
        P1OUT   &= ~BIT0;
        blink_status ^=  0b1;
        if (cursor_status == 1){
            if(blink_status == 1){
                byte(0b0000, 0b1111);
            }
            else{
                byte(0b0000, 0b1110);
            }
        }
        else{
            if(blink_status == 1){
                byte(0b0000, 0b1101);
            }
            else{
                byte(0b0000, 0b1100);
            }
        }
    }
    else{byte(0b0101, 0b1000);}

    return_home();

    // end write key
    return 0;
}
int byte(int upper, int lower){
    upper = upper<<4;
    lower = lower<<4;
    __delay_cycles(5000);
    P1OUT   &= ~0b11110000;
    P1OUT   |= upper;
    P2OUT   |= BIT0;
    __delay_cycles(4000);
    P2OUT   &= ~BIT0;
    __delay_cycles(2000);

    __delay_cycles(2000);
    P1OUT   &= ~0b11110000;
    P1OUT   |= lower;
    P2OUT   |= BIT0;
    __delay_cycles(4000);
    P2OUT   &= ~BIT0;
    __delay_cycles(5000);
    // End DD Ram set key-------------------------------------------
    return 0;
}

int shift_right(void){
    P1OUT   &= ~BIT0;
    P1OUT   &= ~BIT1;
    byte(0b0001,0b0100);
    return 0;
}

int shift_left(void){
    P1OUT   &= ~BIT0;
    P1OUT   &= ~BIT1;
    byte(0b0001,0b0000);
    return 0;
}

int clear_display(void){
    P1OUT   &= ~BIT0;
    P1OUT   &= ~BIT1;
    // Clear display
    byte(0b0000, 0b0001);

    // End Clear display -------------------------------------------
}

int return_home(void){
    P1OUT   &= ~BIT0;
    P1OUT   &= ~BIT1;
    // Return Home -------------------------------------------
    byte(0b0000, 0b0010);

    // End Return Home -------------------------------------------
}
int setup(void){
    // 4 Bit Mode -----------------------------------------
    P1OUT   &= ~BIT0;
    P1OUT   &= ~BIT1;

    byte(0b0010, 0b1100);

    //  End 4 Bit Mode -----------------------------------------

    clear_display();

    return_home();

    // Display on -------------------------------------------
    byte(0b0000, 0b1111);

    //End Display on  -------------------------------------------
    return 0;
}

#pragma vector=EUSCI_B0_VECTOR
__interrupt void LED_I2C_ISR(void){
    switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG)) {
        case USCI_NONE: break;

        case USCI_I2C_UCSTTIFG:   // START condition
            Data_Cnt = 0;         // Reset buffer index
            break;

        case USCI_I2C_UCRXIFG0:   // Byte received
            if (Data_Cnt < 3) {
                Data_In[Data_Cnt++] = UCB0RXBUF;
            }
            if (Data_Cnt == 3) {
                process_i2c_data();  
            }
            break;

        case USCI_I2C_UCSTPIFG:   // STOP condition
            UCB0IFG &= ~UCSTPIFG;  // Clear stop flag
            break;

        default: break;
    }
}

