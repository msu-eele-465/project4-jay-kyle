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
	P6DIR   |= 0b1111111;
    P6OUT   &= ~0b1111111;

	setup();
    i2c_b0_init();

	while(1){
	    clear_display();
	    print_pattern(7);
	    print_key('A');
	    print_key('B');
	    print_key('C');
	    print_key('D');
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
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
        byte(0b0111,0b0011);
        byte(0b0111,0b0100);
        byte(0b0110,0b0001);
        byte(0b0111,0b0100);
        byte(0b0110,0b1001);
        byte(0b0110,0b0011);
    }
    else if(pattern == 1){
        return_home();
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
        byte(0b0111,0b0100);
        byte(0b0110,0b1111);
        byte(0b0110,0b0111);
        byte(0b0110,0b0111);
        byte(0b0110,0b1100);
        byte(0b0110,0b0101);
    }
    else if(pattern == 2){
        return_home();
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
        byte(0b0111,0b0101);
        byte(0b0111,0b0000);
        shift_right();
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
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
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
        byte(0b0110,0b1001);
        byte(0b0110,0b1110);
        shift_right();
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
        byte(0b0110,0b0001);
        byte(0b0110,0b1110);
        byte(0b0110,0b0100);
        shift_right();
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
        byte(0b0110,0b1111);
        byte(0b0111,0b0101);
        byte(0b0111,0b0100);

    }
    else if(pattern == 4){
        return_home();
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
        byte(0b0110,0b0100);
        byte(0b0110,0b1111);
        byte(0b0111,0b0111);
        byte(0b0110,0b1110);
        shift_right();
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
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
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
        byte(0b0111,0b0010);
        byte(0b0110,0b1111);
        byte(0b0111,0b0100);
        byte(0b0110,0b0001);
        byte(0b0111,0b0100);
        byte(0b0110,0b0101);
        shift_right();
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
        byte(0b0011,0b0001);
        shift_right();
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
        byte(0b0110,0b1100);
        byte(0b0110,0b0101);
        byte(0b0110,0b0110);
        byte(0b0111,0b0100);
    }
    else if(pattern == 6){
        return_home();
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
        byte(0b0111,0b0010);
        byte(0b0110,0b1111);
        byte(0b0111,0b0100);
        byte(0b0110,0b0001);
        byte(0b0111,0b0100);
        byte(0b0110,0b0101);
        shift_right();
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
        byte(0b0011,0b0111);
        shift_right();
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
        byte(0b0111,0b0010);
        byte(0b0110,0b1001);
        byte(0b0110,0b0111);
        byte(0b0110,0b1000);
        byte(0b0111,0b0100);
    }
    else if(pattern == 7){
        return_home();
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
        byte(0b0110,0b0110);
        byte(0b0110,0b1001);
        byte(0b0110,0b1100);
        byte(0b0110,0b1100);
        shift_right();
        P6OUT   |= BIT6;
        P6OUT   &= ~BIT5;
        byte(0b0110,0b1100);
        byte(0b0110,0b0101);
        byte(0b0110,0b0110);
        byte(0b0111,0b0100);
    }
    return 0;
}

int print_key(int key){
    // DD Ram set key-------------------------------------------
    P6OUT   &= ~BIT6;
    byte(0b1100, 0b1111);
    // End DD Ram set key-------------------------------------------

    // write Key
    P6OUT   |= BIT6;
    P6OUT   &= ~BIT5;
    if(key == 'A'){byte(0b0100, 0b0001);}

    else if(key == 'B'){byte(0b0100, 0b0010);}

    else if(key == 'C'){
        byte(0b0100, 0b0011);
        P6OUT   &= ~BIT6;
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
        P6OUT   &= ~BIT6;
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
    __delay_cycles(5000);
    P6OUT   &= ~0b1111;
    P6OUT   |= upper;
    P6OUT   |= BIT4;
    __delay_cycles(4000);
    P6OUT   &= ~BIT4;
    __delay_cycles(2000);

    __delay_cycles(2000);
    P6OUT   &= ~0b1111;
    P6OUT   |= lower;
    P6OUT   |= BIT4;
    __delay_cycles(4000);
    P6OUT   &= ~BIT4;
    __delay_cycles(5000);
    // End DD Ram set key-------------------------------------------
    return 0;
}

int shift_right(void){
    P6OUT   &= ~BIT6;
    P6OUT   &= ~BIT5;
    byte(0b0001,0b0100);
    return 0;
}

int shift_left(void){
    P6OUT   &= ~BIT6;
    P6OUT   &= ~BIT5;
    byte(0b0001,0b0000);
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
