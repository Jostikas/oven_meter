#ifndef __DSLEEP_H
#define __DSLEEP_H


#include <c_types.h>
#include "user_interface.h"
#include "logging.h"
#include "rtc_mem_map.h"

#define MLN 1000000ULL

typedef enum ds_rf_mode_t {
	DS_RF_MODE_DEFAULT 	= 0,
	DS_RF_MODE_AS_POR	= 1,
	DS_RF_MODE_NOCAL	= 2,
	DS_RF_MODE_OFF		= 4
} ds_rf_mode_t;

extern bool ICACHE_FLASH_ATTR _dsleep_us(uint64_t sleep_time_us, ds_rf_mode_t rf_mode);

extern ICACHE_FLASH_ATTR ds_rf_mode_t dsleep_get_rf_mode();

#define dsleep_ms(sleep_time_ms, rf_mode)	_dsleep_us(sleep_time_us * 1000ULL, rf_mode)
#define dsleep_s(sleep_time_s, rf_mode)	_dsleep_us(sleep_time_s * MLN, rf_mode)
#define	reinit_with_rf_mode(rf_mode)	_dsleep_us(1ULL, rf_mode)

#endif //__DSLEEP_H