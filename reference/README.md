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
  - Update Line 9-10 in `reference/application/Makefile` according to the path of opencv on your machine.
- On Windows: TBD

## Compile and run

- Enter the application directory and compile the code with an optimization option. Available options:
  - NAIVE: basic implementation.
  - UNROLL: with loop unrolling.
  - REORDERING: with loop reordering (TODO).
  - MULTITHREADING: with multithreading (TODO).
  - SIMD: with simd (TODO).
  - OPT: with all optimization (TODO).

```bash
cd application
make OPTFLAG=NAIVE
```

- Run the demo
  ```bash
  ./main
  ```
