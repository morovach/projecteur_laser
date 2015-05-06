#if defined(__dsPIC33F__)
#include "p33fxxxx.h"
#elif defined(__PIC24H__)
#include "p24hxxxx.h"
#endif


unsigned char S3Flag, S4Flag, S5Flag, S6Flag;

void configure_uart()
{

	// configure U2MODE
	U1MODEbits.UARTEN = 0;	// Bit15 TX, RX DISABLED, ENABLE at end of func
	//U2MODEbits.notimplemented;	// Bit14
	U1MODEbits.USIDL = 0;	// Bit13 Continue in Idle
	U1MODEbits.IREN = 0;	// Bit12 No IR translation
	U1MODEbits.RTSMD = 0;	// Bit11 Simplex Mode
	//U2MODEbits.notimplemented;	// Bit10
	U1MODEbits.UEN = 0;		// Bits8,9 TX,RX enabled, CTS,RTS not
	U1MODEbits.WAKE = 0;	// Bit7 No Wake up (since we don't sleep here)
	U1MODEbits.LPBACK = 0;	// Bit6 No Loop Back
	U1MODEbits.ABAUD = 0;	// Bit5 No Autobaud (would require sending '55')
	U1MODEbits.URXINV = 0;	// Bit4 IdleState = 1  (for dsPIC)
	U1MODEbits.BRGH = 0;	// Bit3 16 clocks per bit period
	U1MODEbits.PDSEL = 0;	// Bits1,2 8bit, No Parity
	U1MODEbits.STSEL = 0;	// Bit0 One Stop Bit


    //U1BRG = BRGVAL or    BRGVAL=((FP/BAUDRATE)/16)-1=> deduce U1BRG
    U1BRG = 256; // Baud Rate setting for 9600



	// Load all values in for U1STA SFR
	U1STAbits.UTXISEL1 = 0;	//Bit15 Int when Char is transferred (1/2 config!)
	U1STAbits.UTXINV = 0;	//Bit14 N/A, IRDA config
	U1STAbits.UTXISEL0 = 0;	//Bit13 Other half of Bit15
	//U2STAbits.notimplemented = 0;	//Bit12
	U1STAbits.UTXBRK = 0;	//Bit11 Disabled
	U1STAbits.UTXEN = 0;	//Bit10 TX pins controlled by periph
	U1STAbits.UTXBF = 0;	//Bit9 *Read Only Bit*
	U1STAbits.TRMT = 0;	//Bit8 *Read Only bit*
	U1STAbits.URXISEL = 0;	//Bits6,7 Int. on character recieved
	U1STAbits.ADDEN = 0;	//Bit5 Address Detect Disabled
	U1STAbits.RIDLE = 0;	//Bit4 *Read Only Bit*
	U1STAbits.PERR = 0;		//Bit3 *Read Only Bit*
	U1STAbits.FERR = 0;		//Bit2 *Read Only Bit*
	U1STAbits.OERR = 0;		//Bit1 *Read Only Bit*
	U1STAbits.URXDA = 0;	//Bit0 *Read Only Bit*

	IFS0bits.U1TXIF = 0;	// Clear the Transmit Interrupt Flag
	IEC0bits.U1TXIE = 1;	// Enable Transmit Interrupts
	IFS0bits.U1RXIF = 0;	// Clear the Recieve Interrupt Flag
	IEC0bits.U1RXIE = 1;	// Enable Recieve Interrupts


  
    S4Flag = 0;
    U1MODEbits.UARTEN = 1;	// And turn the peripheral ON
    U1STAbits.UTXEN = 1;



    S3Flag = 1;

}


void __attribute__((__interrupt__)) _U1RXInterrupt(void)
{
    if (S4Flag == 1) {
        S3Flag =~ S3Flag;
    }
    S4Flag = 1;

    char received_data;

    //Read the Receive FIFO
    received_data = U1RXREG;

    // Transmit one character
    U1TXREG = 'b';

    // Clear RX Interrupt flag
    IFS0bits.U1RXIF = 0; 
    
}


void __attribute__((__interrupt__)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0; // Clear TX Interrupt flag
}

/* ISR ROUTINE FOR THE TIMER1 INTERRUPT */
void __attribute__((interrupt,no_auto_psv)) _T1Interrupt( void )
{
    static unsigned int Count;
    IFS0bits.T1IF = 0;
    T1CONbits.TON = 0;
    if (S3Flag == 1) {
        Count++;
    }
    if(Count == 1000)
    {
        Count		   = 0;
        /*blink a led*/
        LATAbits.LATA0 = ~LATAbits.LATA0;
        U1TXREG = 'a'; // Transmit one character
    }

    /* reset Timer 1 interrupt flag */
    TMR1 = 0;
    T1CONbits.TON = 1;
}
