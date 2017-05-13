Eduroam ESP8266 meat thermometer.
===

This is a web-enabled meat thermometer for people who (like me)
live on the other end of the dorm from the kitchen, and don't
have a common (or transparent) wifi network between their dorm
room and the kitchen.

This repo contains the ESP8266-side code only.
* Hardware design (Kicad schematics and protoboard layout) is at https://github.com/Jostikas/oven_meter_HW
* Server code (Firebase Functions) not yet uploaded
* Android app code not yet uploaded

Building
---
To build you need:
* [esp-open-sdk](https://github.com/pfalcon/esp-open-sdk) built for [ESP8266-NONOS-SDK 2.1.0](https://github.com/espressif/ESP8266_NONOS_SDK/tree/v2.1.0). Actually, it should work fine with 2.0.0, I just built for 2.1.0 to see if they have fixed the wpa2 implementation.
* [libmirom.a](https://github.com/espressif/ESP8266_RTOS_SDK/blob/master/lib/libmirom.a) from [ESP8266-RTOS-SDK](https://github.com/espressif/ESP8266_RTOS_SDK)
* ... I think that's it for external dependencies
* You need to edit your wpa2.a file in esp-open-sdk/sdk/lib by replacing "anonymous@espressif.com with your username@example.edu zero-padded to 23 bytes.
* Rename [`user_config_default.h`](./user/user_config_default.h) to `user_config.h`. This is so I don't accidentally submit my password to github...

IMPORTANT NOTE
---
This is completely unsafe. Due to persistency not being available for WPA2-AES-PEAP connections in the current ESP SDK, nor a user-addressable locked memory the username and password have to be stored in plaintext in the code, or passed in externally every time (which defeats the purpose). So anyone who wants and knows, can pick up the ESP, dump its memory and read your institution password from it. 

It's possible to obscure it, but then you'd have to roll your own obscuration (kind of defeats the purpose of posting your obscuration details on GitHub). Also, I guess it would be possible to store the authentication details in RTC RAM and initialize via an app on a SoftAP-connected phone. I don't know whether that's actually any more secure, but at least it wouldn't be visisble in a flash dump and would be cleared on more than a few seconds of power loss.

Then again, once an attacker has physical access, it's almost moot anyway. The best you can hope for is to only give one chance to get things right. Hmm... wonder if RTC_mem survives entering mode(1,6)...

Other stuff
---
with thanks to many people, who I may not remember, but include:
* Joost van Dijk for [reference code](https://github.com/joostd/esp8266-eduroam "esp8266-eduroam on GitHub") and a helpful [bbs.espressif.com forum post](http://bbs.espressif.com/viewtopic.php?t=2904#p11053) about eduroam
* Paul Sokolovsky and other contributors to esp-open-sdk
* Espressif, although I have many gripes about their SDK.

Currently no licence file, but consider everything under Creative Commons CC BY-NC-SA 4.0 for now.
