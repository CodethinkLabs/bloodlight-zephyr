# Zephyr Support

## Current status

This is a work in progress section. Currently, in order to be able to use the bloodlight board with zephyr a little workaround is needed.

1. Follow the Zephy [getting started](pw) steps.
2. Ensure the following environment variables are set:

```sh
    export ZEPHYR_BASE=<path of zephyr project..>
    export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
    export ZEPHYR_SDK_INSTALL_DIR=<toolchain location..>
```

  Example of the Zephyr environment variable settings:
```
    export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
    export ZEPHYR_SDK_INSTALL_DIR="/opt/zephyr-sdk/"
    export ZEPHYR_BASE=/home/rdale/projects/co054/src/zephyrproject/zephyr

```

3. Build with west:

```sh
    cd $ZEPHYR_BASE
    west build <bloodlight-zephr directory path/${Desired app}>
```

If you would like to make a clean build use

```sh
    west build -p always <bloodlight-zephr directory path/${Desired app}>
```

4. Or build with cmake if preferred:

```sh
    cd <bloodlight-zephr directory path>
    mkdir build
    cd build
    cmake ..
    make
```

Now you can run some basic tests to make sure the LEDs and USB work:

1. Flash the board with the 'zephyr.elf' executable in build/zephyr
2. The test program in src/main.c should print hello_world via ttyACM* (ttyACM2 in my case), and make the blue LED blink

## Board Setup

Refer to [bloodlight-firmware/hardware-setup](https://github.com/CodethinkLabs/bloodlight-firmware#hardware-setup) for infromation on how to setup the board.

## Weird Problem fixing

If you run into unexplicable problems during the build or flash process try the next to fix them.

**SAVE CHANGES FIRST!!!**

- Option 1:
    1. In `zephyrproject/zephyr` execute `git clean -fd`.
    2. `west update`

- Option 2: Delete `zephyrproject/` and `zephyr-sdk` directories and install zephyr back.
