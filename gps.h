/**
 * File: main.c
 * Authors: George Main IV CM 1791 and Hussein Alawami CM 1504
 * 
 * Header file for gps.c
 *
 **/

#ifndef GPS_H
#define	GPS_H

#include <xc.h>
#include "main.h"

// Preprocessor Directives
#define PARSE_4(X)  data[X] * 1000 + data[X + 1] * 100 + data[X + 2] * 10 + data[X + 3]
#define PARSE_3(X)  data[X] * 100 + data[X + 1] * 10 + data[X + 2]
#define PARSE_2(X)  data[X] * 10 + data[X + 1]

inline void readGPS(uint8_t *, uint8_t *);
inline void parseGPS(GPS *);

#endif	/* GPS_H */

