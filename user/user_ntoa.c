#include <math.h>

#include "osapi.h"

#include "user_ntoa.h"

#if DEBUG
	#include "logging.h"
#endif



char* u64toa(uint64_t value, char str[])
{
	char high_count = 0;
	uint32_t low = (uint32_t)(value/4294967296);
	uint32_t high = (uint32_t)(value%4294967296);
	if (high)
	{
		high_count = os_sprintf(str, "%u", high);
	}
	high_count += os_sprintf(str + high_count, "%u", low);
	return str;
}

char* ftoa(float value, char str[], char decimals)
{
	char* ret = str;  // store original address for returning
	uint32_t temp_int;
	if (value < 0)  // Handle negatives
	{
		os_strcpy(str++, "-");
		value = -value;
	}
	if (isnan(value))  //Nadle NaN
	{
		os_strcpy(str, "nan");
		str += 3;
		return ret;
	}
	if (value > __UINT32_MAX__)  // Handle bounds
	{
		os_strcpy(str, "inf");
		str += 3;
		return ret;
	}
	temp_int = (uint32_t) value;  // handle the integral part
	str += os_sprintf(str, "%u", temp_int);
	value -= temp_int;	// handle decimal part
	value = value * pow(10, decimals);
	temp_int = (uint32_t) value;
	os_strcpy(str++, ".");
	str += os_sprintf(str, "%0*u", decimals, temp_int);
	return ret;
}

#if DEBUG
void ftoa_test() 
{
	LOG_D("0 = %s", ftoa(0.0, (char[15]){}, 5));
    LOG_D("-1.254 = %s", ftoa(-1.254, (char[15]){}, 5));
    LOG_D("nan = %s", ftoa((0.0/0.0), (char[15]){}, 5));
    LOG_D("inf = %s", ftoa(1.1*__UINT32_MAX__, (char[15]){}, 5));
    LOG_D("-inf = %s", ftoa(-1.1*__UINT32_MAX__, (char[15]){}, 5));
    LOG_D("big = %s", ftoa(1.0*__UINT32_MAX__, (char[15]){}, 5));
}
#endif  //DEBUG