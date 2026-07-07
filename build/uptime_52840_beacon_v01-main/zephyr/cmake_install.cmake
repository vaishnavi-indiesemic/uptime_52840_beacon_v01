# Install script for directory: C:/ncs/v2.9.2/zephyr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Zephyr-Kernel")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "MinSizeRel")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/ncs/toolchains/b620d30767/opt/zephyr-sdk/arm-zephyr-eabi/bin/arm-zephyr-eabi-objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/zephyr/arch/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/zephyr/lib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/zephyr/soc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/zephyr/boards/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/zephyr/subsys/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/zephyr/drivers/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/nrf/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/mcuboot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/mbedtls/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/trusted-firmware-m/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/cjson/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/azure-sdk-for-c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/cirrus-logic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/openthread/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/suit-processor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/memfault-firmware-sdk/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/canopennode/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/chre/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/lz4/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/nanopb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/zscilib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/cmsis/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/cmsis-dsp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/cmsis-nn/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/fatfs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/hal_nordic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/hal_st/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/hal_wurthelektronik/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/hostap/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/libmetal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/liblc3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/littlefs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/loramac-node/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/lvgl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/mipi-sys-t/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/nrf_wifi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/open-amp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/picolibc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/segger/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/tinycrypt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/uoscore-uedhoc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/zcbor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/nrfxlib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/nrf_hw_models/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/modules/connectedhomeip/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/zephyr/kernel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/zephyr/cmake/flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/zephyr/cmake/usage/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("D:/cust/sva_uptime_in_flash_10_uart_off/uptime_52840_beacon_v01-main/build/uptime_52840_beacon_v01-main/zephyr/cmake/reports/cmake_install.cmake")
endif()

