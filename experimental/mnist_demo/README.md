# Image Classification with MNIST on laptop/desktop

This demo is currently on the dev/platform-independent-vww branch. Please run the following to clone the repo and switch to the branch.

```bash
git clone --recursive https://github.com/mit-han-lab/tinyengine.git
cd tinyengine
git checkout dev/platform-independent-vww
```

## Run the demo with MacOS

- Requirement
  - Opencv: The application needs opencv in order to show images and prediction results, so let's install opencv first.
  ```bash
  brew install opencv
  ```
- Update Line 8 in `experimental/mnist_demo/application/Makefile` according to the path of opencv on your machine. (use `brew info opencv` to get the installation path.)
- Enter the application directory, compile the code, and run the demo.
  ```bash
  cd experimental/mnist_demo/application
  make
  ./main
  ```

Note: The current implementation only supports Intel devices due to its SIMD dependency. Support for M1 chip is still on-going.

## Run the demo with Windows

- Requirement

  - Visual studio (ver. 2022 is recommended)
  - Opencv 4.7.0
    - Option 1: Use the prebuilt the opencv dependency
    ```bash
    cd experimental/mnist_demo/windows_visual_studio_mnist
    unzip opencv_deps.zip
    ```
  - Option 2: Build and opencv from the source (refer the official guide) and put the generated libraries and headers in the following structure.
    ```
    ├── ...
    ├── mnist_demo
    │   ├── windows_visual_studio_mnist
    │   │   ├── opencv_deps                    # Visual studio project directory
    │   │   │   ├── include                    # Header files
    │   │   │   │   └── opencv2
    │   │   │   |       ├── opencv.hpp
    │   │   │   |       └── ...
    │   │   │   ├── lib                        # Static libraries
    │   │   │   |   ├── opencv_core470.lib
    │   │   │   |   └── ...
    │   │   │   └── bin                        # Dynamic libraries
    │   │   │       ├── opencv_core470.dll
    │   │   │       └── ...
    │   │   ├── main.cc                        # Entry point of the windows program
    │   │   └── ...
    │   └── ...
    └── ...
    ```

- Open the project (experimental/mnist_demo/windows_visual_studio.sln) with visual studio

- Choose `Release` and run the demo
