//--------------------------------------------------------------------
//-- Name:	Chris Coulston
//-- Date:	Feb 25, 2015
//-- File:	lec18.c
//-- Event:	Lecture 18
//-- Crs:	ECE 383
//--
//-- Purp:	Examines how to interface custom IP to microBlaze.
//--
//-- Documentation:	I borrowed some techniques from my ECE382 examples
//--
//-- Academic Integrity Statement: I certify that, while others may have
//-- assisted me in brain storming, debugging and validating this program,
//-- the program itself is my own work. I understand that submitting code
//-- which is the work of other individuals is a violation of the honor
//-- code.  I also understand that if I knowingly give my original work to
//-- another individual is also a violation of the honor code.
//-------------------------------------------------------------------------
#include <xuartlite_l.h>					// Contains XUartLite_RecvByte
#include <xparameters.h>
#include <xil_io.h>							// Contains Xil_Out8 and its variations
#include <stdio.h>							// Contains xil_printf

#define	uartReadReg		0x84000000			// read <= RX, write=>TX

#define countQReg		0x83000000			// 8 LSBs of slv_reg0 read=Q, write=D
#define	countCrtlReg	0x83000004			// 2 LSBs of slv_reg2 are control

#define count_HOLD		0x00				// The control bits are defined in the VHDL
#define	count_COUNT		0x01				// code contained in lec18.vhdl.  They are
#define	count_LOAD		0x02				// added here to centralize the bit values in
#define count_RESET		0x03				// a single place.

int main(void) {

    u8  c;

    while(1) {

    	c=XUartLite_RecvByte(uartReadReg);

    	switch(c) {

    		//-------------------------------------------------
    		// Reply with the help menu
    		//-------------------------------------------------
    		case '?':
    			xil_printf("--------------------------\r\n");
    			xil_printf("	count Q = %x\r\n",Xil_In16(countQReg));
    			xil_printf("--------------------------\r\n");
    			xil_printf("?: help menu\r\n");
    			xil_printf("o: k\r\n");
    			xil_printf("c: COUNTER	count up LEDs (by 9)\r\n");
    			xil_printf("l: COUNTER	load counter\r\n");
    			xil_printf("r: COUNTER	reset counter\r\n");
    			xil_printf("f: flush terminal\r\n");
    			break;

    		//-------------------------------------------------
    		// Basic I/O loopback
    		//-------------------------------------------------
    		case 'o':
    			xil_printf("k \r\n");
    			break;

    		//-------------------------------------------------
    		// Tell the counter to count up
    		//-------------------------------------------------
    		case 'c':
    			Xil_Out8(countCrtlReg,count_COUNT);
    			Xil_Out8(countCrtlReg,count_HOLD);
    			break;

        	//-------------------------------------------------
        	// Tell the counter to load a value
        	//-------------------------------------------------
        	case 'l':
        		xil_printf("Enter a 0-9 value to store in the counter: ");
            	c=XUartLite_RecvByte(uartReadReg) - 0x30;
        		Xil_Out8(countQReg,c);					// put value into slv_reg1
        		Xil_Out8(countCrtlReg,count_LOAD);				// load command
    			xil_printf("%c\r\n",c+0x30);
        		break;

            //-------------------------------------------------
            // Reset the counter
            //-------------------------------------------------
            case 'r':
            	Xil_Out8(countCrtlReg,count_RESET);				// reset command
            	break;

            //-------------------------------------------------
            // Clear the terminal window
            //-------------------------------------------------
            case 'f':
            	for (c=0; c<40; c++) xil_printf("\r\n");
               	break;

          //-------------------------------------------------
          // Unknown character was
          //-------------------------------------------------
    		default:
    			xil_printf("unrecognized character: %c\r\n",c);
    			break;
    	} // end case
    } // end while 1
    return 0;
} // end main
