/**
 * File: main.c
 * Authors: George Main IV CM 1791 and Hussein Alawami CM 1504
 * 
 * Header file for uart.c
 *
 **/

#ifndef UART_H
#define	UART_H

#include <xc.h>

#define BAUD_CONFIG 207;        // 9600 baud

inline void initUART(void);
inline void sendMessage(unsigned char *);
inline void sendChar(unsigned char);

#endif	/* UART_H */

