/**
 * File: main.c
 * Authors: George Main IV CM 1791 and Hussein Alawami CM 1504
 * 
 * Header file for tmr1.c
 *
 **/

#ifndef TMR1_H
#define	TMR1_H

#include <xc.h>

#define T1CON_MASK  0xB1
#define CCP1_MASK   0b00001010

inline void initTMR1(void);

#endif	/* TMR1_H */

