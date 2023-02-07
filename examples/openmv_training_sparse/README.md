# Training on OpenMV Cam H7

This is an example showing how to train a model using a predefined sparse update schema with TinyEngine.

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
cd tinyengine/examples/openmv_training_sparse/
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

1. disable some features in the firmware for SRAM and flash space
1. setup for TinyEngine source
1. add the application code for training in `exampleemodule.c`

```
git apply ../openmv_sparse_training.patch
```

## Generate model-specific code and recompile the firmware with TinyEngine

```
cd ..
sh gen_code.sh
cd openmv
make -j4 TARGET=OPENMV4 -C src
```

Flash the binary `openmv/src/build/bin/firmware.bin` into your OpenMV. Please refer to the official [Instructions](https://github.com/openmv/openmv/blob/master/src/README.md#flashing-the-firmware%5D).

## Connect two buttons to your board

Connect two buttons with jump wires to pin1 and pin4. Please refer to the [pinout](http://wiki.amperka.ru/_media/products:openmv-cam-h7:openmv-cam-h7-pinout.pdf).

These two buttons will be used to label images captured by the camera.
![image](https://user-images.githubusercontent.com/17592131/217367877-6a500f31-be3b-4258-a86e-4eabbb947a7e.png)

## Start the demo

1. Open OpenMV IDE
1. Connect your OpenMV cam to the PC
1. Run the python script `tinyengine/examples/openmv_vww/vww_openmv_demo.py` in OpenMV IDE.
