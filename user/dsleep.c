#include "dsleep.h"
#include "user_ntoa.h"


bool ICACHE_FLASH_ATTR _dsleep_us(uint64_t sleep_time_us, ds_rf_mode_t rf_mode) 
{
	char sleep_time_str[21];
	LOG_D("[_dsleep_us]");
	LOG_D("[ -> sleep_time_us: %s]", 
		u64toa(sleep_time_us, (char[21]){}));  //Nice, C99!
	LOG_D("[ -> rf_mode: %d", rf_mode);
	if (!system_deep_sleep_set_option(rf_mode)) {
		LOG_I("Failed setting RF wakeup state. Aborting");
		return;
	} 

	if ( (sleep_time_us / system_rtc_clock_cali_proc()) << 12 > __UINT32_MAX__) {
		LOG_I("Sleep time too long. See SDK ref for max sleep time. Aborting");
		return;
	}
	if (!system_rtc_mem_write(RTCMEM_DS_RF_MODE, &rf_mode, sizeof(ds_rf_mode_t))) {
		LOG_I("Failed storing RF wakeup state in memory. Aborting");
		return;
	}
	LOG_D("cali: %d", system_rtc_clock_cali_proc());
	return system_deep_sleep(sleep_time_us);
}

ds_rf_mode_t ICACHE_FLASH_ATTR dsleep_get_rf_mode() 
{
	LOG_D("[dsleep_get_rf_state]");
	ds_rf_mode_t rf_mode;
	system_rtc_mem_read(RTCMEM_DS_RF_MODE, &rf_mode, sizeof(ds_rf_mode_t));
	LOG_D("[dsleep_get_rf_state] = %d", rf_mode);
	return rf_mode;
}