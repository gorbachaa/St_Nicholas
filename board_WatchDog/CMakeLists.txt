cmake_minimum_required(VERSION 3.16)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE cmake/arm-gcc-toolchain.cmake)
endif()

#-------------------------------------------------------------------------------
function(glob_src VARNAME src_path)
    file(GLOB_RECURSE _glob_src
            ${src_path}/*.cpp
            ${src_path}/*.cc
            ${src_path}/*.c
            ${src_path}/*.s
            ${src_path}/*.S
            ${src_path}/*.h
            ${src_path}/*.hpp
        )

    set(${VARNAME} ${_glob_src} PARENT_SCOPE)
endfunction(glob_src)

macro(reject_file_pattern pattern input_list)
  foreach(src_file ${${input_list}})
    get_filename_component(base_name ${src_file} NAME)
    if(${base_name} MATCHES ${pattern})
      list(REMOVE_ITEM ${input_list} "${src_file}")
    endif()
  endforeach()
endmacro()

macro(reject_dir_pattern pattern input_list)
  foreach(src_file ${${input_list}})
    get_filename_component(base_name ${src_file} DIRECTORY)
    if(${base_name} MATCHES ${pattern})
      list(REMOVE_ITEM ${input_list} "${src_file}")
    endif()
  endforeach()
endmacro()
#-------------------------------------------------------------------------------

if(NOT (EXISTS ${RELEASE_DIR}))
    set(RELEASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/release)
endif()

set(_pro_name St_Nicholas)
project(${_pro_name} VERSION 0.1.0 LANGUAGES C CXX ASM)
set(TARGET_NAME ${_pro_name})
set(TARGET_ELF ${_pro_name}.elf)
if ( PROJECT_VERSION_TWEAK)
    set(TARGET_NAME_VER ${PROJECT_NAME}-v${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${PROJECT_VERSION_TWEAK})
else()
    set(TARGET_NAME_VER ${PROJECT_NAME}-v${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
endif()

set(LD_FILE ${CMAKE_CURRENT_SOURCE_DIR}/STM32F042F6Px_FLASH.ld)
set(VECT_TAB_OFFSET_VALUE 0)
set(HSE_VALUE             8000000)

set(BOARD_NAME "st-nicklas-0" CACHE STRING "Name of board builds for.")
mark_as_advanced(BOARD_NAME)
set(BOARD_VERSION 0)

set(COMM_BAUDRATE 115200)
set(DEBUG_BAUDRATE 115200)
set(MCU STM32F042x6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if( NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

option(USE_USB_LOG "Enable logging through USB-CDC" OFF)

#------------ FLAGS ------------------------------------------------------------
set(MCU_ARCH      -mcpu=cortex-m0 -mthumb)
set(COMMON_FLAGS  -ffunction-sections -fdata-sections -flto -fno-builtin)
set(SPECS         -specs=nano.specs -specs=rdimon.specs)

SET(CMAKE_MODULE_LINKER_FLAGS "${MCU_ARCH}")
SET(CMAKE_SHARED_LINKER_FLAGS "${MCU_ARCH}")
#-------------------------------------------------------------------------------

set(SRC_LIST
    ${LD_FILE}
    ${CMAKE_CURRENT_SOURCE_DIR}/startup_stm32f042x6.s
)

glob_src(_list_src_1 ${CMAKE_CURRENT_SOURCE_DIR}/Inc)
glob_src(_list_src_2 ${CMAKE_CURRENT_SOURCE_DIR}/Src)
glob_src(_list_src_3 ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares)
glob_src(_list_src_4 ${CMAKE_CURRENT_SOURCE_DIR}/Drivers)
list(APPEND SRC_LIST ${_list_src_1})
list(APPEND SRC_LIST ${_list_src_2})
list(APPEND SRC_LIST ${_list_src_3})
list(APPEND SRC_LIST ${_list_src_4})

reject_file_pattern( template  SRC_LIST)
reject_dir_pattern( Templates  SRC_LIST)
reject_dir_pattern( DSP_Lib  SRC_LIST)
reject_dir_pattern( RTOS  SRC_LIST)


SOURCE_GROUP(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC_LIST})

set(IDE_LIST
    README.md
)

add_executable(${TARGET_ELF} ${SRC_LIST} ${SRC_COMMON} ${IDE_LIST})
target_compile_options(${TARGET_ELF}
    PRIVATE
        ${MCU_ARCH}
        ${COMMON_FLAGS}
        ${SPECS}
        -Wall -Wextra
        $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions -fno-rtti -fno-unwind-tables -fno-non-call-exceptions -fno-use-cxa-atexit>
)
target_link_options(${TARGET_ELF}
    PRIVATE
        ${MCU_ARCH}
        ${SPECS}
        -flto
        -Wl,--gc-sections
        -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.map,--cref
        -T ${LD_FILE}
)
target_include_directories(${TARGET_ELF}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F0xx_HAL_Driver/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F0xx_HAL_Driver/Inc/Legacy
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F0xx/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
)
target_compile_definitions(${TARGET_ELF}
    PRIVATE
        ${MCU}
        MCU_STR="${MCU}"
        __SOFTFP__
        USE_HAL_DRIVER
        USE_FULL_LL_DRIVER
        #USE_FULL_ASSERT
        #HSE_VALUE=${HSE_VALUE}

        COMM_BAUDRATE=${COMM_BAUDRATE}
        DEBUG_BAUDRATE=${DEBUG_BAUDRATE}
        $<$<COMPILE_LANGUAGE:CXX>:register={}>
        $<IF:$<BOOL:${USE_USB_LOG}>,USE_USB_LOG,>
)

add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE_DIR:${TARGET_ELF}>/${TARGET_NAME}.hex"
    COMMAND ${CMAKE_OBJCOPY} -O ihex "$<TARGET_FILE:${TARGET_ELF}>" "${TARGET_NAME}.hex"
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_ELF}>"
)

add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE_DIR:${TARGET_ELF}>/${TARGET_NAME}.bin"
    COMMAND ${CMAKE_OBJCOPY} -O binary "$<TARGET_FILE:${TARGET_ELF}>" "${TARGET_NAME}.bin"
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_ELF}>"
)

add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE_DIR:${TARGET_ELF}>/${TARGET_NAME}.lss"
    COMMAND ${CMAKE_OBJDUMP} -dC ${TARGET_ELF} | "\"${CMAKE_CPPFILT}\"" > ${TARGET_NAME}.lss
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_ELF}>"
)

add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "------------------------- SIZE of ${TARGET_ELF} :"
    COMMAND ${CMAKE_SIZE_UTIL} -B "${TARGET_ELF}"
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_ELF}>"
)

add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Release dir: $<TARGET_FILE_DIR:${TARGET_ELF}>"
    COMMAND ${CMAKE_COMMAND} -E echo "release ${TARGET_NAME}.hex "
    COMMAND ${CMAKE_COMMAND} -E echo "release ${TARGET_NAME}.bin "
    #COMMAND ${CMAKE_COMMAND} -E copy
    #    "$<TARGET_FILE_DIR:${TARGET_ELF}>/${TARGET_NAME}.bin"
    #    "${RELEASE_DIR}/${TARGET_NAME}.bin"
    #COMMAND ${CMAKE_COMMAND} -E echo "release ${TARGET_NAME_VER}.bin "
    #COMMAND ${CMAKE_COMMAND} -E copy
    #    "$<TARGET_FILE_DIR:${TARGET_ELF}>/${TARGET_NAME}.bin"
    #    "${RELEASE_DIR}/${TARGET_NAME_VER}.bin"
)

