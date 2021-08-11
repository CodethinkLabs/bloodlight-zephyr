# Bloodlight-Zephyr

## Introduction

This projects aims to develop firmware support for Codethink's [Bloodlight board](https://github.com/CodethinkLabs/bloodlight-hardware)
using [Zephyr RTOS](https://www.zephyrproject.org/). To do so, we take as a reference the [Bloodlight-firmware](https://github.com/CodethinkLabs/bloodlight-firmware) project and we first replace [Libopencm3](https://libopencm3.org/) library's APIs with Zephyr APIs. Sometimes this is not straighforward and we can't just replace one by the other because they don't share identical behaviours. When this happens, some workarounds are put in place to be able to use the Zephyr API without modifying too much the original function's behaviour.

## Current status

So far, we currently have completed:

- Added Zephyr board support for bloodlight_rev2 board. For information on how to add board support read Zephyr's [Board Porting Guide](https://docs.zephyrproject.org/latest/guides/porting/board_porting.html?highlight=board). To check the actual support work check [boards/arm/bloodlight_rev2](boards/arm/bloodlight_rev2).

- Configure CMake to be able to build the app with the board support files stored in this repo isntead of the Zephyr's repository tree.

- Move bloodlight-firmware code here to start migrating it to zephyr.
  - Currently this are the bits that have been sucessfully migrated to Zephyr:
    - Led Support.
  - Currently this are the bits that we are working on migrating to Zephyr:
    - USB Support.

- Add tests for:
  - LEDs.

## Future Tasks

- Implement zephyr's Ztest framework in our tests. This work has been already started but its on hold. See [Zephyr ztest for LEDs](https://github.com/CodethinkLabs/bloodlight-zephyr/issues/16).
- [Replace GPIO port idx identifier with zephyrs one](https://github.com/CodethinkLabs/bloodlight-zephyr/issues/14).
- Add support for reading ADC using Zephyr
- Get sensor readings working with the opamps
- Add DMA support to stm32 ADC driver in Zephyr. This work was started a while ago but it was put on hold by this project, you might want to check first if this was already achieved in Zephyr before working on this. Check [this branch](https://github.com/ikerperezdelpalomar/zephyr/commits/adc_dma) for the started work and [Add VREF, TEMPSENSOR, VBAT internal channels to the stm32 adc driver](https://github.com/zephyrproject-rtos/zephyr/issues/17375) for information.
- Implement DMA in out readings.

More information in [Task-Planning](https://github.com/CodethinkLabs/bloodlight-zephyr/issues/11).

## Getting Started

Currently, in order to be able to use the bloodlight board with zephyr a little workaround is needed.

1. **Follow Zephyr's [Getting Started](https://docs.zephyrproject.org/latest/getting_started/index.html#) steps**.
2. **Ensure the following environment variables are set:**

    ```sh
        export ZEPHYR_BASE=<path of zephyr project..>
        export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
        export ZEPHYR_SDK_INSTALL_DIR=<toolchain location..>
    ```

      Example of the Zephyr environment variable settings:

    ```sh
        export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
        export ZEPHYR_SDK_INSTALL_DIR="/opt/zephyr-sdk/"
        export ZEPHYR_BASE=/home/rdale/projects/co054/src/zephyrproject/zephyr
    ```

3. **Build**

    - Option 1: Build with `west`.

        ```sh
            cd $ZEPHYR_BASE
            west build <bloodlight-zephr directory path/${Desired app}>
        ```

        Current working app is `tests/led/`.

        If you would like to make a clean build use

        ```sh
            west build -p always <bloodlight-zephr directory path/${Desired app}>
        ```

    - Option 2: Build with cmake if preferred:

        ```sh
            cd <bloodlight-zephr directory path>
            mkdir build
            cd build
            cmake ..
            make
        ```

4. **Board Setup**: Please refer to Bloodlight-firmware's Readme [Hardware Setup](https://github.com/CodethinkLabs/bloodlight-firmware#hardware-setup) section for an explanation about how to do this.

5. **Flash**

    ```sh
        west flash
    ```

 *`tests/led/` program should report via `ttyACM*` (ttyACM2 in my case), and make the LEDs blink.*

## Weird Problem fixing

If you run into unexplicable problems during the build or flash process try the next to fix them.

**SAVE CHANGES FIRST!!!**

- Option 1:
    1. In `zephyrproject/zephyr` execute `git clean -fd`.
    2. `west update`

- Option 2: Delete `zephyrproject/` and `zephyr-sdk` directories and install zephyr back.