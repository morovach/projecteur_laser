/* 
 * File:   uart.h
 * Author: Jérémie
 *
 * Created on 6 mai 2015, 10:28
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

#define FP 40000000
#define BAUDRATE 9600
#define BRGVAL ((FP/BAUDRATE)/16)-1
#define DELAY_105uS asm volatile ("REPEAT, #4201"); Nop(); // 105uS delay


void configure_uart();
void __attribute__((__interrupt__)) _U1RXInterrupt(void);
void __attribute__((__interrupt__)) _U1TXInterrupt(void);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

