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

```sh
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

## Objective

The idea is to have a `Zephyr` out of tree app. To achieve this we will first need to establish a connection between our board support files and zephyr and then between our app and zephyr.

The second task won't be hard, but the first one it might be a bit more tricky, since so far I haven't found an example of a running app that uses a board that is not supported by zephyr.

## Testing

In order to test all the available test for this project execute the next:

```sh
    cd $ZEPHYR_BASE
    ./scripts/twister --list-tests -T <bloodlight-zephr directory path>/tests
```

To run all tests:

```sh
    cd $ZEPHYR_BASE
    ./scripts/twister --device-testing --device-serial /dev/ttyACM2 -p bloodlight_rev2  -T <bloodlight-zephr directory path>/tests --board-root=~/<bloodlight-zephr directory path>/boards
```