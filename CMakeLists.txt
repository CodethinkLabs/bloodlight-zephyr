# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

# Re-direct the directory where the 'boards' directory is found from
# $ZEPHYR_BASE to this directory.
set(BOARD_ROOT ${CMAKE_CURRENT_LIST_DIR})
set(BOARD bloodlight_rev2)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Specify directory with includes in 'common'
zephyr_include_directories(${CMAKE_SOURCE_DIR})

project(bloodlight_rev2)

target_sources(app PRIVATE 
    src/bl.c
#    src/delay.c
    src/led.c
#    src/mq.c
#    src/msg.c
#    src/spi.c
#    src/spi-test.c
#    src/tick.c
#    src/usb.c
#    src/acq/adc.c
#    src/acq.c
#    src/acq/channel.c
#    src/acq/dac.c
#    src/acq/dma.c
#    src/acq/opamp.c
#    src/acq/source.c
#    src/acq/timer.c
)

add_compile_definitions(
    BL_REVISION=2
    REVISION=2
)