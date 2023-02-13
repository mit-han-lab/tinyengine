# Face mask detection on OpenMV Cam H7

This is an example showing how to deploy the face mask detection model on OpenMV Cam H7 with TinyEngine.

![demo](../../assets/figures/face_mask_demo.gif)

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
cd examples/openmv_face_mask_detection/
git clone https://github.com/openmv/openmv.git
```

Currently, we don't have compatibility tests for the OpenMV source, so let's use the version that has been manually tested before.

```
cd tinyengine/examples/openmv_face_mask_detection/openmv
git checkout 918ccb937730cc759ee5709df089d9de516dc7bf
git submodule update --init --recursive
```

## Build the source

Let's first build the firmware from the source to make sure all required dependencies are correctly installed. The `TARGET `is set to `OPENMV4` for OpenMV Cam H7.

```
cd tinyengine/examples/openmv_face_mask_detection/openmv
make -j4 -C src/micropython/mpy-cross
make -j4 TARGET=OPENMV4 -C src
```

You should see the compiled binary at `openmv/src/build/bin/firmware.bin`.

## Apply customized patch

The patch is to

1. disable some features in the firmware for SRAM and Flash space
1. setup for TinyEngine source
1. add the detection application code in `exampleemodule.c`

```
cd tinyengine/examples/openmv_face_mask_detection/openmv
git apply ../openmv_face_mask_detection.patch
```

## Generate model-specific code for face mask detection

```
cd tinyengine/examples/openmv_face_mask_detection
sh gen_code.sh
```

Copy the generated code at `tinyengine/example/openmv_face_mask_detection/codegen` into the openvm source.

## Recompile the firmware with TinyEngine

```
cd openmv/
make -j4 TARGET=OPENMV4 -C src
```

Flash the binary `openmv/src/build/bin/firmware.bin` into your OpenMV Cam. Please refer to the official [Instructions](https://github.com/openmv/openmv/blob/master/src/README.md#flashing-the-firmware%5D).

## Start the demo

1. Open OpenMV IDE
1. Connect your OpenMV cam to the PC
1. Run the python script `tinyengine/examples/openmv_face_mask_detection/face_detection_demo.py ` in OpenMV IDE.
