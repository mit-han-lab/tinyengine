# Deploy VWW on OpenMV Cam H7

This is an example showing how to deploy the VWW model on OpenMV Cam H7 with TinyEngine.

## Install build dependencies on Linux

Note: This section is basically from https://github.com/openmv/openmv/blob/master/src/README.md. Please refer to the OpenMV's repo for more details or steps on different env.

```
sudo apt-get update
sudo apt-get install git build-essential
```

## Install GNU ARM toolchain

```
Install arm toolchain
TOOLCHAIN_PATH=/usr/local/arm-none-eabi
TOOLCHAIN_URL="https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu-rm/10-2020q4/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux.tar.bz2"
sudo mkdir ${TOOLCHAIN_PATH}
wget --no-check-certificate -O - ${TOOLCHAIN_URL} | sudo tar --strip-components=1 -jx -C ${TOOLCHAIN_PATH}
export PATH=${TOOLCHAIN_PATH}/bin:${PATH}
```

## Clone the OpenMV source

```
cd tinyengine/examples/openmv_vww/
git clone https://github.com/openmv/openmv.git
```

Currently, we don't have compatibility tests for the OpenMV source, so let's use the version that has been manually tested before.

```
cd openmv
git checkout 918ccb937730cc759ee5709df089d9de516dc7bf
git submodule update --init --recursive
```

## Build the source

Let's first build the firmware from the source to make sure all required dependencies are correctly installed. The `TARGET `is set to `OPENMV4` for OpenMV Cam H7.

```
make -j4 -C src/micropython/mpy-cross
make -j4 TARGET=OPENMV4 -C src
```

You should see the compiled binary at `openmv/src/build/bin/firmware.bin`.

## Apply customized patch

The patch is to

1. disable some features in the firmware for SRAM and Flash space
1. setup for TinyEngine source
1. add vww application code in `exampleemodule.c`

```
cd tinyengine/examples/openmv_vww/openmv
git apply ../openmv.patch
```

# Add the Tinyengine into openmv

```
cd tinyengine
cp -r TinyEngine examples/openmv_vww/openmv/src/omv/modules/
```

## Generate model-specific code for VWW

```
cd tinyengine/examples/openmv_vww/
python ../vww.py
cp -r codegen/ openmv/src/omv/modules/TinyEngine/
```

Copy the generated code at `tinyengine/example/openmv_vww/codegen` into TinyEngie.

## Recompile the firmware with TinyEngine

```
cd tinyengine/examples/openmv_vww/openmv/
make -j4 TARGET=OPENMV4 -C src
```

Flash the binary `openmv/src/build/bin/firmware.bin` into your OpenMV. Please refer to the official [Instructions](https://github.com/openmv/openmv/blob/master/src/README.md#flashing-the-firmware%5D).

## Start the demo

1. Open OpenMV IDE
1. Connect your OpenMV cam to the PC
1. Run the python script `tinyengine/examples/openmv_vww/vww_openmv_demo.py` in OpenMV IDE.
