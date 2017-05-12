#ifndef __USER_NTOA_H
#define __USER_NTOA_H

#include "user_interface.h"

/* Convert a 64-bit unsigned integer to string.

Assumes that the argument buffer can fit the resulting string, which can be up to 20 characters
(plus termination).
*/
extern char* u64toa(uint64_t value, char str[]);


/* Convert a float to string. Values 

Assumes that the argument buffer can fit the resulting string, no bounds checking!
Negative values work.
No exponential representation. 
Bounds (-2^32, 2^32).
Round towards 0.
Not optimized in any way.
Also completely untested.
*/
extern char* ftoa(float value, char str[], char decimals);

#if DEBUG
extern void ftoa_test();
#endif //DEBUG

#endif //__USER_NTOA_H