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

## The code below is meant to obtain the last commit sha and store it for future
## use in msg.c. However, this was giving me many headaches so I decided to
## hard-code a random sha to able to move forward and create an issue in the
## project's repo explaining this and the need to fix it.
##find_package(Git QUIET)
##if (Git_FOUND)
##    execute_process(
##        COMMAND "shell" "git" "rev-parse" "--verify HEAD"
##       WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
##        RESULT_VARIABLE git_result
##        OUTPUT_VARIABLE commit
##        )
##else()
##    message("ERROR: Git wasn't found")
##endif()
##message(STATUS "git ver[${git_result}]: ${commit}")

add_compile_definitions(
    BL_REVISION=2
    REVISION=2
    BL_COMMIT_SHA="57348f49c171e940fc3ecd291d04b2d5cac1ee0f"
)