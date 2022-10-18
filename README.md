# St_Nicholas
Dropping Gifts

This repository contains sources for 3 boards
- WatchDog - https://vataga.ua/product/usb-watchdog-storozhevoy-taymer-dlya-pk-i-ferm/
- ArduinoNano - https://arduino.ua/prod166-arduino-nano-v3-0-avr-atmega328p-s-raspayannimi-razemami
- ATtiny13A - https://imrad.com.ua/ru/attiny13a-pu



### board_WatchDog

### build with make
For build with make you should have proper compiler in PATH env variable.

```sh
  make clean
  make all -j 12
```
### build with cmake
For build with cmake you should have proper compiler in PATH env variable.

```sh
  cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel
  cmake --build build --target all
```


### board_ATtiny13A

### build with make
For build with make you should have proper compiler in PATH env variable.

```sh
  make clean
  make all -j 12
```
### build with cmake
For build with cmake you should have proper compiler in PATH env variable.

```sh
  cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel
  cmake --build build --target all
```


### board_ArduinoNano
 Install Arduino IDE and open scetch






