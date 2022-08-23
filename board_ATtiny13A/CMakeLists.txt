
cmake_minimum_required(VERSION 3.16)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE cmake/avr-gcc-toolchain.cmake)
endif()


##################################################################################
# tools to be used for programming the AVR
##################################################################################
set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER usbasp)
set(AVR_UPLOADTOOL_PORT usb)

#### For Arduino bootloader:
#set(AVR_PROGRAMMER arduino)
#set(AVR_UPLOADTOOL_PORT /dev/tty.wchusbserial1420)
#set(AVR_UPLOADTOOL_BITRATE 57600)


# AVR and fuses needs to be set
set(AVR_MCU attiny13a)
set(MCU_SPEED "9600000UL")
#set(AVR_H_FUSE 0xDA)
#set(AVR_L_FUSE 0xFF)
#set(AVR_EX_FUSE 0x05)

option(USE_UART_LOG "Enable logging through UART" OFF)
if(USE_UART_LOG)
   add_definitions("-DUSE_UART_LOG")
endif(USE_UART_LOG)

### printf support
#set(EXTRA_FLAGS "-Wl,-u,vfprintf -lprintf_min") 

### END TOOLCHAIN SETUP AREA #############################################


##########################################################################
# name your project
##########################################################################
set(_pro_name St_Nicholas)
project(${_pro_name} VERSION 0.1.0 LANGUAGES C CXX ASM)
set(TARGET_NAME ${_pro_name})
set(TARGET_ELF ${_pro_name}.elf)
if ( PROJECT_VERSION_TWEAK)
    set(TARGET_NAME_VER ${PROJECT_NAME}-v${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${PROJECT_VERSION_TWEAK})
else()
    set(TARGET_NAME_VER ${PROJECT_NAME}-v${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
endif()

set(BOARD_NAME "board_ATtiny13A" CACHE STRING "Name of board builds for.")
mark_as_advanced(BOARD_NAME)
set(BOARD_VERSION 0)

##################################################################################
# add all sources and headers from ./src
##################################################################################
file(GLOB_RECURSE SRC_C_FILES ${PROJECT_SOURCE_DIR}/Src/*.c)
file(GLOB_RECURSE SRC_CXX_FILES ${PROJECT_SOURCE_DIR}/Src/*.cpp)
file(GLOB_RECURSE SRC_HEADERS ${PROJECT_SOURCE_DIR}/Src/*.h)

##################################################################################
# add AVR executable
##################################################################################
add_avr_executable(
   ${PROJECT_NAME}
   ${SRC_C_FILES} ${SRC_CXX_FILES} ${SRC_HEADERS}
)

find_library(C_LIB c)
message(STATUS "avr-libc: ${C_LIB}")

avr_target_link_libraries(${PROJECT_NAME} ${C_LIB})


##################################################################################
# add AVR library
##################################################################################
#add_avr_library(
#   <library-name>
#   <sources> <headers>
#)

##################################################################################
# link library to executable
# NOTE: It needs to be the elf target.
##################################################################################
# target_link_libraries(<executable-name>-${AVR_MCU}.elf <library-name>-${AVR_MCU})
# OR easier
