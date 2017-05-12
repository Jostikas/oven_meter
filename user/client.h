#ifndef __CLIENT_H
#define __CLIENT_H

#include <stdbool.h>

#include "user_interface.h"

#include "user_config.h"



/* Post stored temperature from RTC memory to server.
Returns true if sucessful, else false.
*/
extern bool ICACHE_FLASH_ATTR client_post_temp();

extern bool ICACHE_FLASH_ATTR client_announce();

#endif //__CLIENT_H