#include <msp430.h> 

    int pattern = 0;
    int cursor_status = 1;
    int blink_status = 1;
/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	//-- Setup Ports

	P6DIR   |= 0b1111111;
    P6OUT   &= ~0b1111111;
	PM5CTL0 &= ~LOCKLPM5;


	setup();




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
