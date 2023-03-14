# Image Classification with MNIST on laptop/desktop

This demo is currently on the dev/platform-independent-vww branch. Please run the following to clone the repo and switch to the branch.

```bash
git clone --recursive https://github.com/mit-han-lab/tinyengine.git
cd tinyengine
git checkout dev/platform-independent-vww
```

## Dependency

- On MacOS: The application needs opencv in order to access your camera.
  - Install opencv
  ```bash
  brew install opencv
  ```
  - Update Line 8 in `reference/mnist_demo/application/Makefile` according to the path of opencv on your machine. (use `brew info opencv` to get the installation path.)
- On Windows: TBD

## Compile and run

- Enter the application directory and compile the code with an optimization option. Available options:
  - NAIVE: basic implementation.
  - UNROLL: with loop unrolling.
  - UNROLL_SIMD: with loop unrolling and SIMD intrinsic instructions.

```bash
cd reference/mnist_demo/application
make OPTFLAG=UNROLL_SIMD
```

- Run the demo
  ```bash
  ./main
  ```
