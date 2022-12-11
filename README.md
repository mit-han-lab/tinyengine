# TinyEngine

This is the official implementation of TinyEngine, a memory-efficient and high-performance neural network library for Microcontrollers.
TinyEngine is a part of MCUNet, which also consists of TinyNAS. MCUNet is a system-algorithm co-design framework for tiny deep learning on microcontrollers. TinyEngine and TinyNAS are co-designed to fit the tight memory budgets.

**The MCUNet and TinyNAS repo is [here](https://github.com/mit-han-lab/mcunet).**

### [MCUNetV1](https://mcunet.mit.edu/#mcunetv1) | [MCUNetV2](https://mcunet.mit.edu/#mcunetv2) | [MCUNetV3](https://mcunet.mit.edu/#mcunetv3)

### [Demo (Inference)](https://www.youtube.com/watch?v=YvioBgtec4U)

![demo](assets/figures/mcunet_demo.gif)

### [Demo (Training)](https://www.youtube.com/watch?v=XaDCO8YtmBw)

![demo_v3](assets/figures/mcunetV3_demo_2images.gif)

## News

We will soon release the **INT8 verson of Tiny Training Engine** used in [MCUNetV3: On-Device Training Under 256KB Memory](https://mcunet.mit.edu/#mcunetv3). **If you are interested in getting updates, please sign up [here](https://forms.gle/UW1uUmnfk1k6UJPPA) to get notified!**

- **(2022/12)** We release the source code for patch-based inference, and include the [example script](examples/vww_patchbased.py) that generates patch-based inference code for the VWW demo.
- **(2022/11)** We release the source code of the FP32 version of Tiny Training Engine, and include the [tutorial of our training demo](tutorial/training) for training a visual wake words (VWW) model on microcontrollers. The INT8 version will also be released soon, so please stay tuned!
- **(2022/11)** We release the source code of the algorithm and compilation parts of MCUNetV3 in [this repo](https://github.com/mit-han-lab/tiny-training). Please take a look!
- **(2022/10)** Our new work [On-Device Training Under 256KB Memory](https://arxiv.org/abs/2206.15472) is highlighted on the [MIT homepage](http://web.mit.edu/spotlight/learning-edge/)!
- **(2022/09)** Our new work [On-Device Training Under 256KB Memory](https://arxiv.org/abs/2206.15472) is accepted to NeurIPS 2022! It enables tiny on-device training for IoT devices \[[demo](https://www.youtube.com/watch?v=XaDCO8YtmBw)\].
- **(2022/08)** Our **New Course on TinyML and Efficient Deep Learning** will be released soon in September 2022: [efficientml.ai](https://efficientml.ai/).
- **(2022/08)** We include the [tutorial of our inference demo](tutorial/inference) for deploying a visual wake words (VWW) model onto microcontrollers.
- **(2022/08)** We opensource the TinyEngine repo.
- **(2022/07)** We include the person detection model used in the video demo above in the [MCUNet repo](https://github.com/mit-han-lab/mcunet).
- **(2022/06)** We refactor the [MCUNet repo](https://github.com/mit-han-lab/mcunet) as a standalone repo (previous repo: https://github.com/mit-han-lab/tinyml)
- **(2021/10)** **MCUNetV2** is accepted to NeurIPS 2021: https://arxiv.org/abs/2110.15352 !
- **(2020/10)** **MCUNet** is accepted to NeurIPS 2020 as **spotlight**: https://arxiv.org/abs/2007.10319 !
- Our projects are covered by: [MIT Spotlight News (v3)](http://web.mit.edu/spotlight/learning-edge/), [MIT News (v2)](https://news.mit.edu/2021/tiny-machine-learning-design-alleviates-bottleneck-memory-usage-iot-devices-1208), [MIT News (v1)](https://news.mit.edu/2020/iot-deep-learning-1113), [WIRED](https://www.wired.com/story/ai-algorithms-slimming-fit-fridge/), [Morning Brew](https://www.morningbrew.com/emerging-tech/stories/2020/12/07/researchers-figured-fit-ai-ever-onto-internet-things-microchips), [Stacey on IoT](https://staceyoniot.com/researchers-take-a-3-pronged-approach-to-edge-ai/), [Analytics Insight](https://www.analyticsinsight.net/amalgamating-ml-and-iot-in-smart-home-devices/), [Techable](https://techable.jp/archives/142462), etc.

## Overview

Microcontrollers are low-cost, low-power hardware. They are widely deployed and have wide applications, but the tight memory budget (50,000x smaller than GPUs) makes deep learning deployment difficult.

MCUNet is a **system-algorithm co-design** framework for tiny deep learning on microcontrollers. It consists of **TinyNAS** and **TinyEngine**. They are co-designed to fit the tight memory budgets. With system-algorithm co-design, we can significantly improve the deep learning performance on the same tiny memory budget.

![overview](assets/figures/overview.png)

Specifically, TinyEngine is a memory-efficient inference library. TinyEngine adapts the memory scheduling according to the overall network topology rather than layer-wise optimization, reducing memory usage and accelerating the inference. It outperforms existing inference libraries such as [TF-Lite Micro](https://www.tensorflow.org/lite/microcontrollers) from Google, [CMSIS-NN](https://arxiv.org/abs/1801.06601) from Arm, and [X-CUBE-AI](https://www.st.com/en/embedded-software/x-cube-ai.html) from STMicroelectronics.

TinyEngine adopts the following optimization techniques to accelerate inference speed and minimize memory footprint.

- [**In-place depth-wise convolution**](https://mcunet.mit.edu/#mcunetv1): A unique data placement technique for depth-wise convolution that overwrites input data by intermediate/output data to reduce peak SRAM memory.
- [**Operator fusion**](https://docs.microsoft.com/en-us/windows/ai/directml/dml-fused-activations): A method that improves performance by merging one operator into a different operator so that they are executed together without requiring a roundtrip to memory.
- [**SIMD (Single instruction, multiple data) programming**](https://en.wikipedia.org/wiki/Single_instruction,_multiple_data): A computing method that performs the same operation on multiple data points simultaneously.
- [**HWC to CHW weight format transformation**](https://oneapi-src.github.io/oneDNN/dev_guide_understanding_memory_formats.html): A weight format transformation technique that increases cache hit ratio for in-place depth-wise convolution.
- [**Image to Column (Im2col) convolution**](https://iq.opengenus.org/im2col/): An implementation technique of computing convolution operation using general matrix multiplication (GEMM) operations.
- [**Loop reordering**](https://xilinx.github.io/Vitis_Accel_Examples/2019.2/html/loop_reorder.html): A loop transformation technique that attempts to optimize a program's execution speed by reordering/interchanging the sequence of loops.
- [**Loop unrolling**](https://en.wikipedia.org/wiki/Loop_unrolling): A loop transformation technique that attempts to optimize a program's execution speed at the expense of its binary size, which is an approach known as space-time tradeoff.
- [**Loop tiling**](https://en.wikipedia.org/wiki/Loop_nest_optimization): A loop transformation technique that attempts to reduce memory access latency by partitioning a loop's iteration space into smaller chunks or blocks, so as to help ensure data used in a loop stays in the cache until it is reused.

![inplace_depthwise](assets/figures/inplace_depthwise.png)

By adopting the abovementioned optimization techniques, TinyEngine can not only enhance inference speed but also reduce peak memory, as shown in the figures below.

**MAC/s improvement breakdown:**
![mac_result](assets/figures/mac_result.png)

**Peak memory reduction:**
![peakmem_result](assets/figures/peakmem_result.png)

To sum up, our **TinyEngine** inference engine could be a useful infrastructure for MCU-based AI applications. It significantly **improves the inference speed and reduces the memory usage** compared to existing libraries like [TF-Lite Micro](https://www.tensorflow.org/lite/microcontrollers), [CMSIS-NN](https://arxiv.org/abs/1801.06601), [X-CUBE-AI](https://www.st.com/en/embedded-software/x-cube-ai.html), etc. It improves the inference speed by **1.1-18.6x**, and reduces the peak memory by **1.3-3.6x**.

![measured_result](assets/figures/measured_result.png)

## Code Structure

`code_generator` contains a python library that is used to compile neural networks into low-level source code (C/C++).

`TinyEngine` contains a C/C++ library that implements operators and performs inference on Microcontrollers.

`examples` contains the examples of transforming TFLite models into our TinyEngine models.

`tutorial` contains the demo tutorial (of inference and training) of deploying a visual wake words (VWW) model onto microcontrollers.

`assets` contains misc assets.

## Requirement

- Python 3.6+
- STM32CubeIDE 1.5+

## Setup for Users

First, clone this repository:

```bash
git clone --recursive https://github.com/mit-han-lab/tinyengine.git
```

(Optional) Using a virtual environment with `conda` is recommended.

```bash
conda create -n tinyengine python=3.6 pip
conda activate tinyengine
```

Install dependencies:

```bash
pip install -r requirements.txt
```

## Setup for Developers

Install pre-commit hooks to automatically format changes in your code.

```
pre-commit install
```

## Deployment Example

Please see [tutorial](tutorial) to learn how to deploy a visual wake words (VWW) model onto microcontrollers by using TinyEngine. We include both [the inference demo](tutorial/inference) and [the training demo](tutorial/training) in the tutorial, please take a look!

## Measured Results

- All the tflite models are from [Model Zoo in MCUNet repo](https://github.com/mit-han-lab/mcunet#model-zoo). Please see MCUNet repo to know how to build the pre-trained int8 quantized models in TF-Lite format.
- All the **latency**, **peak memory (SRAM)** and **Flash memory usage** results are profiled on STM32F746G-DISCO discovery boards.
- Note that we measure the newer versions of libraries in this repo, so that the results in this repo might be different from the ones in the MCUNet papers.
- Since TF-Lite Micro no longer has version numbers anymore, we use the git commit ID to indicate its newer version.
- All the tflite models are compiled by `-Ofast` optimization level in STM32CubeIDE.
- OOM denotes Out Of Memory.

The **latency** results:

| net_id                       | TF-Lite Micro<br>v2.1.0 | TF-Lite Micro<br>[@ 713b6ed](https://github.com/tensorflow/tflite-micro/tree/713b6ed6bd81d8d6906d885e14f444aaf9c154f6) | CMSIS-NN<br>v2.0.0 | X-CUBE-AI<br>v7.1.0 | TinyEngine |
| ---------------------------- | ----------------------- | -------------------------- | ------------------ | --------- | ---------- |
| *# mcunet models (VWW)*      |                         |                            |                    |           |            |
| mcunet-5fps-vww              |          624ms          |          2346ms            |        269ms       |   137ms   |   128ms    |
| mcunet-10fps-vww             |          345ms          |          1230ms            |        143ms       |    76ms   |    66ms    |
| mcunet-320kB-vww             |           OOM           |            OOM             |         OOM        |   657ms   |   570ms    |
| *# mcunet models (ImageNet)* |                         |                            |                    |           |            |
| mcunet-5fps                  |           OOM           |            OOM             |         OOM        |   149ms   |   135ms    |
| mcunet-10fps                 |           OOM           |            OOM             |         OOM        |    84ms   |    62ms    |
| mcunet-256kB                 |           OOM           |            OOM             |         OOM        |   839ms   |   681ms    |
| mcunet-320kB                 |           OOM           |            OOM             |         OOM        |    OOM    |   819ms    |
| *# baseline models*          |                         |                            |                    |           |            |
| mbv2-320kB                   |           OOM           |            OOM             |         OOM        |    OOM    |   292ms    |
| proxyless-320kB              |           OOM           |            OOM             |         OOM        |   484ms   |   425ms    |

The **peak memory (SRAM)** results:

| net_id                       | TF-Lite Micro<br>v2.1.0 | TF-Lite Micro<br>[@ 713b6ed](https://github.com/tensorflow/tflite-micro/tree/713b6ed6bd81d8d6906d885e14f444aaf9c154f6) | CMSIS-NN<br>v2.0.0 | X-CUBE-AI<br>v7.1.0 | TinyEngine |
| ---------------------------- | ----------------------- | -------------------------- | ------------------ | --------- | ---------- |
| *# mcunet models (VWW)*      |                         |                            |                    |           |            |
| mcunet-5fps-vww              |          227kB          |            220kB           |        248kB       |   123kB   |    88kB    |
| mcunet-10fps-vww             |          169kB          |            163kB           |        199kB       |    98kB   |    56kB    |
| mcunet-320kB-vww             |           OOM           |             OOM            |         OOM        |   259kB   |   162kB    |
| *# mcunet models (ImageNet)* |                         |                            |                    |           |            |
| mcunet-5fps                  |           OOM           |             OOM            |         OOM        |   126kB   |    90kB    |
| mcunet-10fps                 |           OOM           |             OOM            |         OOM        |    76kB   |    45kB    |
| mcunet-256kB                 |           OOM           |             OOM            |         OOM        |   311kB   |   200kB    |
| mcunet-320kB                 |           OOM           |             OOM            |         OOM        |    OOM    |   242kB    |
| *# baseline models*          |                         |                            |                    |           |            |
| mbv2-320kB                   |           OOM           |             OOM            |         OOM        |    OOM    |   284kB    |
| proxyless-320kB              |           OOM           |             OOM            |         OOM        |   312kB   |   242kB    |

The **Flash memory usage** results:

| net_id                       | TF-Lite Micro<br>v2.1.0 | TF-Lite Micro<br>[@ 713b6ed](https://github.com/tensorflow/tflite-micro/tree/713b6ed6bd81d8d6906d885e14f444aaf9c154f6) | CMSIS-NN<br>v2.0.0 | X-CUBE-AI<br>v7.1.0 | TinyEngine |
| ---------------------------- | ----------------------- | -------------------------- | ------------------ | --------- | ---------- |
| *# mcunet models (VWW)*      |                         |                            |                    |           |            |
| mcunet-5fps-vww              |          782kB          |             733kB          |        743kB       |   534kB   |   517kB    |
| mcunet-10fps-vww             |          691kB          |             643kB          |        653kB       |   463kB   |   447kB    |
| mcunet-320kB-vww             |           OOM           |              OOM           |         OOM        |   773kB   |   742kB    |
| *# mcunet models (ImageNet)* |                         |                            |                    |           |            |
| mcunet-5fps                  |           OOM           |              OOM           |         OOM        |   737kB   |   720kB    |
| mcunet-10fps                 |           OOM           |              OOM           |         OOM        |   856kB   |   837kB    |
| mcunet-256kB                 |           OOM           |              OOM           |         OOM        |   850kB   |   827kB    |
| mcunet-320kB                 |           OOM           |              OOM           |         OOM        |    OOM    |   835kB    |
| *# baseline models*          |                         |                            |                    |           |            |
| mbv2-320kB                   |           OOM           |              OOM           |         OOM        |    OOM    |   828kB    |
| proxyless-320kB              |           OOM           |              OOM           |         OOM        |   866kB   |   835kB    |

## Citation

If you find the project helpful, please consider citing our paper:

```
@article{
  lin2020mcunet,
  title={Mcunet: Tiny deep learning on iot devices},
  author={Lin, Ji and Chen, Wei-Ming and Lin, Yujun and Gan, Chuang and Han, Song},
  journal={Advances in Neural Information Processing Systems},
  volume={33},
  year={2020}
}

@inproceedings{
  lin2021mcunetv2,
  title={MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning},
  author={Lin, Ji and Chen, Wei-Ming and Cai, Han and Gan, Chuang and Han, Song},
  booktitle={Annual Conference on Neural Information Processing Systems (NeurIPS)},
  year={2021}
}

@article{
  lin2022ondevice,
  title = {On-Device Training Under 256KB Memory},
  author = {Lin, Ji and Zhu, Ligeng and Chen, Wei-Ming and Wang, Wei-Chen and Gan, Chuang and Han, Song},
  journal = {arXiv:2206.15472 [cs]},
  url = {https://arxiv.org/abs/2206.15472},
  year = {2022}
}
```

## Related Projects

[MCUNet: Tiny Deep Learning on IoT Devices](https://mcunet.mit.edu/#mcunetv1) (NeurIPS'20)

[MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning](https://mcunet.mit.edu/#mcunetv2) (NeurIPS'21)

[MCUNetV3: On-Device Training Under 256KB Memory](https://mcunet.mit.edu/#mcunetv3) (NeurIPS'22)
