/**
 * File: tmr1.c
 * Authors: George Main IV CM 1791 and Hussein Alawami CM 1504
 * 
 * This file contains the initialization for Timer1
 *
 **/

#include "tmr1.h"

inline void initTMR1(void) {
    T1CON = T1CON_MASK;
    CCP1CON = CCP1_MASK;
    CCP1IF = 0;
    CCP1IE = 1;
}
