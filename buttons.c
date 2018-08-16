/**
 * File: main.c
 * Authors: George Main IV CM 1791 and Hussein Alawami CM 1504
 * 
 * This file contains initialization for the buttons
 *
 **/

#include "buttons.h"

inline void initButtons(void) {
    TRISB0 = TRISB1 = 1;
    ANS12 = ANS10 = 0;
    nRBPU = 0;
}