# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

# Re-direct the directory where the 'boards' directory is found from
# $ZEPHYR_BASE to this directory.
set(BOARD_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)
set(BOARD bloodlight_rev2)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Specify directory with includes in 'common'
zephyr_include_directories(${CMAKE_SOURCE_DIR}/../..)

project(led_test)

target_sources(app PRIVATE 
    bloodlight_based.c
    ../../src/led.c
)

add_compile_definitions(
    BL_REVISION=2
    REVISION=2
)