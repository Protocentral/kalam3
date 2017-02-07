# ProtoCentral Kalam v3 IOT platform

ProtoCentral Kalam v3 is our newest IOT platform based on the ESP8266 Wi-Fi SoC. It's easy-to-use, low cost and easily scalable. 

IoT is now made easy with quick plug-and-play functionality in both hardware and software. Based on the popular ESP8266 Wi-Fi SoC from Espressif, this board contains everything required, right out of the box.

![Kalam v3](/docs/images/protocentral-kalam-v3.jpg)

Features
--------
* ESP8266EX WiFi System-on-Chip (SoC) with ARM Cortex M3 microcontroller
* On-board 2.4 GHz PCB antenna
* FTDI-based USB-UART interface with microUSB connector
* Fully compatible with the Arduino IDE
* Single Li-Ion battery powered
* On-board battery charger
* 31-pin Hirose DF-9 connector on the back for expansion

Repository Contents
-------------------
* **/hardware** - All Eagle design files (.brd, .sch)
* **/libraries** - Arduino libraries and example code
* **/docs** - Additional Documentation

Board Overview
--------------
![Kalam labelled](/docs/images/kalam-labelled.jpg)

Getting Started with Kalam
--------------------------
Connect the GPIO expaner cap to the Kalam V3 by stacking it on top.Now that the setup is complete,the setup needs to be powered up.Attach a Li-poly battery to the Kalam v2 Iot board.Control the power using the on/off switch

How to setup Arduino support for Sparkfun thing Dev
---------------------------------------------------
Kalam is powered by the Sparkfun's board support file.The first thing you will need to do is to download the latest release of the Arduino IDE. You will need to be using version 1.6.4 or higher. 
After you have downloaded and installed v1.6.4, you will need to start the IDE and navigate to the Preferences menu.
Sparkfun's  JSON url for ESP8266 board is - https://raw.githubusercontent.com/sparkfun/Arduino_Boards/master/IDE_Board_Manager/package_sparkfun_index.json
![Json](/docs/images/json.jpg)

* Click Ok to close and save the preferences
* Then click Tools > Board > Board Manager
* You should find Sparkfun AVR boards listed in the Board Manager window.
  ![board_manager](/docs/images/boardmanager.png)

License Information
-------------------
This product is open source!

Please use, reuse, and modify these files as you see fit. Please maintain attribution to Protocentral and release anything derivative under the same license.
