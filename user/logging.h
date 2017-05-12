#ifndef __LOGGING_H
#define __LOGGING_H

#include "osapi.h"

#include "user_config.h"

#define LOG_D(f_, ...) \
	do {if (DEBUG) ets_printf("[DEBUG] " f_ "\n", ##__VA_ARGS__); } while(0)

#define LOG_I(f_, ...) ets_printf("[INFO ] " f_ "\n", ##__VA_ARGS__)

#endif //__LOGGING_H