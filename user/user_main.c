#include "c_types.h"
#include "ets_sys.h"
#include "eagle_soc.h"
#include "osapi.h"
#include "os_type.h"
#include "gpio.h"
#include "user_interface.h"
#include "esp_sdk_ver.h"

#include "user_config.h"
#include "client.h"
#include "logging.h"
#include "dsleep.h"
#include "rtc_mem_map.h"
#include "user_ntoa.h"

#define WIFI_LED_IO_MUX     PERIPHS_IO_MUX_GPIO2_U
#define WIFI_LED_IO_NUM     2
#define WIFI_LED_IO_FUNC    FUNC_GPIO2

#define SENSOR_EXCITE_IO_MUX	PERIPHS_IO_MUX_GPIO4_U
#define SENSOR_EXCITE_IO_NUM	4
#define SENSOR_EXCITE_IO_FUNC	FUNC_GPIO4

bool initialization = false;

void wifi_handle_event_cb(System_Event_t *evt)
{
    switch (evt->event) {
        case EVENT_STAMODE_CONNECTED:
            LOG_D("[connect to ssid %s, channel %d]",
                evt->event_info.connected.ssid,
                evt->event_info.connected.channel);
            break;
        case EVENT_STAMODE_DISCONNECTED:
            LOG_D("[disconnect from ssid %s, reason %d]",
                evt->event_info.disconnected.ssid,
                evt->event_info.disconnected.reason);
            break;
        case EVENT_STAMODE_AUTHMODE_CHANGE:
            LOG_D("[mode: %d -> %d]",
                evt->event_info.auth_change.old_mode,
                evt->event_info.auth_change.new_mode);
            break;
        case EVENT_STAMODE_GOT_IP:
            LOG_D("[ip:" IPSTR ",mask:" IPSTR ",gw:" IPSTR "]",
                IP2STR(&evt->event_info.got_ip.ip),
                IP2STR(&evt->event_info.got_ip.mask),
                IP2STR(&evt->event_info.got_ip.gw));
            if (initialization) {
            	client_announce();
            } else {
                client_post_temp();
            }
            dsleep_s(DSLEEP_PERIOD_S, DS_RF_MODE_OFF);
            break;
        default:
            LOG_D("[event: %x]\n", evt->event);
            break;
    }
}

void ICACHE_FLASH_ATTR connect()
{
	LOG_D("[connect]");
	LOG_D("[initialization: %d", initialization);
	struct station_config wifi_config;

    if (!wifi_set_opmode_current(STATION_MODE))
    {
        LOG_I("Failed to set station mode.");
    }

	os_memset(&wifi_config,0,sizeof(wifi_config));
    os_memcpy(wifi_config.ssid, SSID, os_strlen(SSID));
    // password is already empty due to struct initialization.
    os_memcpy(wifi_config.password, "", os_strlen(""));
    wifi_station_set_config_current(&wifi_config);

    /* Set up WPA2-AES-PEAP credentials */
    if (wifi_station_set_wpa2_enterprise_auth(1)) {
        LOG_I("[failed to set WPA2-Enterprise]");
    }
    if (wifi_station_set_enterprise_username(PEAP_USER, os_strlen(PEAP_USER))) {
        os_printf("[failed to set username]");
    }
    if (wifi_station_set_enterprise_password(PEAP_PASSWORD, os_strlen(PEAP_PASSWORD))) {
        os_printf("[failed to set password]");
    }

    wifi_set_event_handler_cb(wifi_handle_event_cb);
}

/*Get an adc reading. Assumes that RF is off.*/
uint16_t ICACHE_FLASH_ATTR get_adc_reading() 
{
	LOG_D("[get_adc_reading]");
	uint16_t reading;
	system_adc_read_fast(&reading, 1, 8);
	LOG_D("[get_adc_reading] = %d", reading);
	return reading;
}

float ICACHE_FLASH_ATTR transform_temp(uint16_t reading) 
{
	LOG_D("[transform_temp, %d] NOT IMPLEMENTED", reading);
	float temp = -1.254;
	LOG_D("[transform_temp] = %s", ftoa(temp, (char[5]){}, 2));
	return temp;
}

/* Measure the temperature, store in RTC memory */
void ICACHE_FLASH_ATTR measure_temp() 
{
	LOG_D("[measure_temp]");
	uint16_t reading = get_adc_reading();
	float temp = transform_temp(reading);
	LOG_I("T* = %s", ftoa(temp, (char[5]){}, 2));
	system_rtc_mem_write(RTCMEM_LAST_TEMP, &temp, sizeof(float));
}

/* Choose action to perform after waking up based on whether RF is active or not. */
void ICACHE_FLASH_ATTR measure_or_post_temp() 
{
	ds_rf_mode_t rf_mode = dsleep_get_rf_mode();
	switch (rf_mode) {
		case DS_RF_MODE_OFF:
			measure_temp();
			if (!reinit_with_rf_mode(DS_RF_MODE_AS_POR))
			{
				LOG_I("Failed to enter deep sleep for some reason...");
			}
			LOG_I("Deep sleep returned true");
			break;
		default:
			LOG_D("[Entered default rf_mode case. rf_mode: %d]", rf_mode);
			connect();
			break;
	}
}

void ICACHE_FLASH_ATTR gpio_init() 
{
	wifi_status_led_install(WIFI_LED_IO_NUM, WIFI_LED_IO_MUX, WIFI_LED_IO_FUNC);
}

void ICACHE_FLASH_ATTR user_init(void) 
{
	uart_div_modify(0, UART_CLK_FREQ/115200);

    if (wifi_get_opmode) 
    {
	   wifi_set_opmode(NULL_MODE);
    }

	gpio_init();

	struct rst_info* reason_p = system_get_rst_info();
	switch (reason_p->reason) {
		case REASON_DEFAULT_RST:
		case REASON_EXT_SYS_RST:
			initialization = true;
			LOG_I("\n\nOven meter app starting.");
			LOG_D("SDK version: %s", system_get_sdk_version());
			connect();
			break;
		case REASON_DEEP_SLEEP_AWAKE:
			LOG_D("[Woken]");
			measure_or_post_temp();
			break;
		default:
			LOG_D("Reset with unknown reason: %d", reason_p->reason);
			reinit_with_rf_mode(DS_RF_MODE_OFF);
			break;
	}
}

