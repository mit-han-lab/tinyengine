# Running VWW on laptop/desktop

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
  - Update Line 9 in `experimental/application/Makefile` according to the path of opencv on your machine. (use `brew info opencv` to get the installation path.)
- On Windows: TBD

## Compile and run

- Enter the application directory and compile the code with an optimization option. Available options:
  - NAIVE: basic implementation.
  - UNROLL_TILING: with loop unrolling and tiling.
  - MULTITHREADING: with multithreading.
  - SIMD: with SIMD intrinsic (Intel only).
  - OPT: with all optimization.

```bash
cd experimental/application
make OPTFLAG=NAIVE
```

- Run the demo
  ```bash
  ./main
  ```
