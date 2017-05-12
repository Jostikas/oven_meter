===
Eduroam ESP8266 meat thermometer.
===

This is a web-enabled meat thermometer for people who (like me)
live on the other end of the hall from the kitchen, and don't
have a common (or transparent) wifi network between their dorm
room and the kitchen.

This repo contains the ESP8266-side code only.

---
Building
---
To build you need:
* esp-open-sdk built for ESP-NONOS-SDK 2.1.0
* libmirom.a from ESP-RTOS-SDK
* ... I think that's it for external dependencies
* You need to edit your wpa2.a file in esp-open-sdk/sdk/lib by replacing "anonymous@espressif.com with your username@example.edu zero-padded to 23 bytes.
* Rename user_config_default.h to user_config.h. This is so I don't accidentally submit my password to github...

---
IMPORTANT NOTE
---
This is completely unsafe. Due to persistency not being available for WPA2-AES-PEAP connections in the current ESP SDK, the username and password have to be stored in plaintext in the code, or passed in externally. So anyone who wants and knows, can pick up the ESP, dump its memory and read your password from it.

---
Other stuff
---
with thanks to many people, who I may not remember, but include:
* joostd (for reference code and a helpful bbs.espressif.com forum post about eduroam
* contributors to esp-open-sdk
* Espressif, although I have many gripes about their SDK.

Currently no licence file, but consider everything under Creative Commons CC BY-NC-SA 4.0 for now.
