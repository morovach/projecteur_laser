#if defined(__dsPIC33F__)
#include "p33fxxxx.h"
#elif defined(__PIC24H__)
#include "p24hxxxx.h"
#endif

//The frquency Fosc = 80MHz and the frequency of the CPU is Fosc/2
void configure_oscillator()
{

    //Fosc = Fin * (PLLDIV+2)
    //          ---------------------
    //        ((PLLPRE+2)*2(PLLPOST+1))
    //PLLFBD = PLLDIV
    ///FIN = FRC oscillateur interne = 7.37 MHz

    PLLFBD=41;                          //
    CLKDIVbits.PLLPOST=0;		//
    CLKDIVbits.PLLPRE=0;		//
    CLKDIVbits.FRCDIV=0;                //FRC is not divided in the first divider
    CLKDIVbits.DOZEN= 0;                //FCY=Fosc/2

    // In that configuration: Fosc= 79227500
    // Fcy= 39613750
    // FP = FCY

    // Initiate Clock Switch to Internal FRC with PLL (NOSC = 0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(OSCCON | 0x01);
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0b001);
    // Wait for PLL to lock
    while(OSCCONbits.LOCK!=1) {};
    RCONbits.SWDTEN=0;            /* Disable Watch Dog Timer*/


}


void configure_pinout()
{


    /*This commands is here to unlock the IOLOCK register
     When this register is = 1, it is unauthorised to modify the RP pins
     * and to remap the IOs */
    __builtin_write_OSCCONL(OSCCON & 0xDF);

    /*Initialize the Ports */
    TRISA = 0x0000;
    LATA  = 0x0000;
    PORTA = 0x0000;

    //Only RB1 (RP1) in RX mode
    TRISB = 0;
    TRISBbits.TRISB1 = 1;
    // Connect U1TX on RP0
    RPOR0bits.RP0R = 3;
    //The input RP11 is connected to U1RX
    RPINR18bits.U1RXR = 1;

    //ALL ADC pins are rather connected to IOs.
    //The ADC IOs are by default connected to the analog input, if we want to use them as a logical
    //IO, we need to write 1 to the corresponding bit
    AD1PCFGL = 0xFF;

    /*Lock again the PIN remapping for security*/
    __builtin_write_OSCCONL(OSCCON | (1<<6));

}