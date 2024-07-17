---
layout: post
title: CUDA并行程序设计
index_img: /img/post_pics/cuda/book.jpeg
date: 2024-04-04 10:28:15
tags: 
    - 并行计算
    - GPU
    - CUDA
categories: 
    - GPU
---

## 目录

- [目录](#目录)
- [System info](#system-info)
- [Helloworld](#helloworld)
- [带上这些问题](#带上这些问题)
- [1. 超级计算](#1-超级计算)
  - [cuda的其他选择](#cuda的其他选择)
- [2. 使用GPU理解并行计算](#2-使用gpu理解并行计算)
  - [串行/并行](#串行并行)
  - [并发](#并发)
  - [局部性](#局部性)
  - [并行处理的类型](#并行处理的类型)
    - [基于任务](#基于任务)
    - [基于数据](#基于数据)
  - [弗林分类法](#弗林分类法)
  - [常用的并行模式](#常用的并行模式)
    - [基于循环](#基于循环)
    - [派生/汇集](#派生汇集)
    - [分条/分块](#分条分块)
    - [分而治之](#分而治之)
- [3. CUDA硬件概述](#3-cuda硬件概述)
- [5. 线程网络、线程块和线程](#5-线程网络线程块和线程)
  - [线程](#线程)
    - [问题分解](#问题分解)
    - [GPU和CPU区别](#gpu和cpu区别)
    - [任务执行模式](#任务执行模式)
    - [GPU线程](#gpu线程)
      - [内存空间](#内存空间)
      - [线程ID](#线程id)
    - [硬件实现](#硬件实现)
    - [CUDA内核](#cuda内核)
  - [线程块](#线程块)
    - [线程块的分配](#线程块的分配)
  - [线程网格](#线程网格)
    - [跨幅和偏移](#跨幅和偏移)
    - [X与Y方向的线程索引](#x与y方向的线程索引)
  - [线程束](#线程束)
    - [分支](#分支)
    - [GPU的利用率](#gpu的利用率)
  - [线程块的调度](#线程块的调度)
  - [Ex.直方图](#ex直方图)

## System info

Based on [Jetson Orin Nano](https://www.nvidia.com/en-us/autonomous-machines/embedded-systems/jetson-orin/).

- GPU: 1024-core NVIDIA Ampere architecture GPU with 32 Tensor Cores
- AI Performance: 40 TOPS
- GPU Max Frequency: 625MHz
- CPU: 6-core Arm® Cortex®-A78AE v8.2 64-bit CPU 1.5MB L2 + 4MB L3
- CPU Max Frequency: CPU Max Frequency
- Memory: 8GB 128-bit LPDDR5 68 GB/s
- CSI Camera: 2x MIPI CSI-2 22-pin Camera Connectors
- Power: 7W - 15W

```bash
nvcc --version
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2022 NVIDIA Corporation
Built on Sun_Oct_23_22:16:07_PDT_2022
Cuda compilation tools, release 11.4, V11.4.315
Build cuda_11.4.r11.4/compiler.31964100_0
```

## Helloworld

```c
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

__global__ void cuda_hello(){
    printf("Hello world from GPU! %d\n", threadIdx.x * gridDim.x);
}

int main(){
    printf("Hello world from gpu!\n");
    cuda_hello<<<500, 1024>>>();
    cudaDeviceSynchronize();
    return 0;
}
```

## 带上这些问题

更多：

<https://blog.csdn.net/m0_38086244/article/details/133791115>
<https://zhuanlan.zhihu.com/p/51402722>
<https://github.com/DefTruth/CUDA-Learn-Note>
LLM论文：<https://github.com/DefTruth/Awesome-LLM-Inference>

涉及岗位：异构计算/AI框架研发/高性能计算/模型部署/算法优化/算子研发

- cpu与gpu的区别？
  CPU的设计着重于处理单个线程的复杂计算和控制流程。
  GPU 被设计用于高密度和并行计算，更多的晶体管投入到数据处理而不是数据缓存和流量控制
  体现在GPU的ALU（算术逻辑运算单元）数量更多

- cuda编程中的SM SP 是什么?
  SP(streaming processor)，计算核心，最基本处理单元
  SM(Streaming multiprocessor)，多个SP加上其他找资源组成一个SM

- cuda编程的内存模型
  全局内存
  共享内存
  寄存器

- cuda编程的软件模型
  Block,线程块
  Grid,线程格
  thread，线程

- stream（cuda 流）概念的理解
  主机发出的在一个设备中执行的CUDA操作（和CUDA有关的操作，包括主机-设备数据传输和kerenl执行)

- 使用共享内存时需要注意什么？
  （1）线程同步
  __syncthreads() 在利用共享内存进行线程块之间合作通信前，都要进行同步，以确保共享内存变量中的数据对线程块内的所有线程来说都准备就绪
  （2）避免共享内存的 bank 冲突
  bank 冲突概念：同一线程束内的多个线程试图访问同一个 bank 中不同层的数据时，造成bank冲突
  只要同一线程束内的多个线程不同时访问同一个 bank 中不同层的数据，该线程束对共享内存的访问就只需要一次内存事务

- 对一个cuda kernel的进行优化可以从哪些角度入手

- GPU L1/L2缓存介绍
- 同步stream和异步stream的理解

手撕

- 矩阵乘
- softmax规约
- nchw转nhwc
- 长度为n的数组 统计每个元素出现的频率 元素大小为0-256 统计的频率放在另一个数组里面
  将数组里面的奇数位置的数放在左边，偶数位置的数放在右边，原地操作

## 1. 超级计算

- 内存速度和时钟速度的比率是限制CPU和GPU吞吐量的一个重要因素。

- `缓存命中`：当处理器从缓存而不是主存中取来一条指令或一个数据时，称为缓存命中(cachehit)。但是随着缓存容量的不断增大，使用更大容量缓存所带来的增速收益却迅速下降。

- `边际效益`：在计算领域里，你经常能遇见“收益递减规律”(Thelawofdiminishingreturs)。即便你在一个单一方面投入再多，结果也没有太大改变，因为它受限于成本、空间、电力供应、散热等因素。解决办法是在各个影响因素之间选择一个平衡点，然后多次地复制它。

- `功耗墙`：如果CPU产生的热量增加，那么即使时钟频率不变，根据硅材料的性质，CPU的功耗也会进一步增加。这个电/热转换是对能源的一个浪费。这个不断增加的无效的电能消耗，意味着你要么不能充分为处理器提供电力，要么不能够有效地冷却处理器，已经达到了电子设备或芯片封装的散热极限，即所谓的“功耗墻”(powerwall)。

`图形处理器(GraphicsProcessingUnit, GPU)`是现代PC机中的常见设备。它们向CPU提供一些基本的操作，比如，对内存中的图像进行着色，然后将其显示在屏幕上。一个GPU通常会处理一个复杂的多边形集合，即需要着色的图片映像，然后给这些多边形涂上图片的纹理，进而再做阴影和光照处理。重要的进步之一就是可编程着色器(programmableshader)的出现。它们是GPU运行的一些用来计算各种图片效果的小程序。这样，着色就不必固定在GPU中进行。通过可下载的着色器，就可以完成这些操作。这就是最初的`通用图形处理器(General-PurposeGraphicalProcessorUnit, GPGPU)`编程。这表示GPU设计朝着“处理单元功能不再是固定的”方向迈出了第一步。而CUDA可能是首次实现了上述目标，同时向程序员提供了一个真正通用的GPU编程语言。英伟达发现了一个能使GPU进入主流的契机，那就是为GPU增加一个易用的编程接又，也就是所请的`统一计算架构(ComputeUnifiedDeviceArchitecture, CUDA)`。这为无须学习复杂的着色语言或者图形处理原语，就能进行GPU编程提供了可能。

现代GPU的体系结构也完全相同。一个GPU内有许多`流处理器簇(Streaming Multiprocessor, SM)`，它们就类似CPU的核。这些SM与共享存储(一级缓存)连接在一起，然后又与相当于SM间互联开关的二级缓存相连。数据先是存储在全局存储中，然后被主机取出并使用。每个图形卡由若干个流处理器簇(SM)组成，每个SM配备8个或更多的`流处理器(Stream Processor, SP)`。每个GPU设备包括一组SM，每个SM又包含一组SP或者CUDA核。这些SP最高可以实现32个单元并行工作。它们消除了CPU上为实现高速串行执行而设计的大量复杂的指令级并行处理电路。取而代之的是由程序员指定的显式的并行模型，使得同样大小的硅片可以容纳更强的计算能力。GPU的总体性能主要由其**所具有的SP的数量、全局内存的带宽和程序员利用并行架构的充分程度**等因素决定。

### cuda的其他选择

OpenCL，DirectCompute，ROCm

## 2. 使用GPU理解并行计算

`MPI(Message Passing Interface)`标准用于解决节点间的并行处理，常用于定义良好的网络内的计算机集群。它常用于由几千个节点组成的超级计算机系统。每个节点只分担问题的一小部分。

多核处理器主要用于实现`任务并行(task parallelism)`，这是一种基于操作系统的并行处理。

`线程`是程序中一个独立的执行流，它可以在主执行流的要求下分出或聚合。**通常情况下，CPU程序拥有的活动线程数量，不超过其包含的物理处理核数量的两倍。**就像在单核处理器中，操作系统的任务是分时、轮流运行的，每个任务只能运行一小段时间，从而实现数量多于物理CPU核数的众多任务同时运行。随着线程数量的增加，终端用户也开始感觉到线程的存在。因为在后台，操作系统需要频繁进行`上下文切换(即对于一组寄存器进行内容的换入、换出)`。而“上下文切换”是一项很费时的操作，通常需要几千个时钟周期，所以**CPU的应用程序的线程数要尽量比GPU的少**。

### 串行/并行

线程会引起并行程序设计中的许多问题，例如，资源的共享。通常，这个问题用`信号灯(semaphore)`来解决，而最简单的信号灯就是一个`锁或者令牌(token)`。只有拥有令牌的那个线程可以使用资源，其他线程只能等待，直到这个线程释放令牌。因为只有一个令牌，所以所有工作都有条不紊地进行。这是基于线程操作的一个让人为难的地方。线程操作的是一个共享的内存空间，这既可以带来不借助消息就可以完成数据交换的便利，也会引起缺乏对共享数据保护的问题。可以用进程来替换线程。不过，因为代码和数据的上下文都必须由操作系统保存，所以操作系统装入进程就要吃力得多。相比之下，只有线程代码的上下文(一个程序或指令计数器加上一组寄存器)由操作系统保存，而数据空间是共享的。总之，进程和线程可以在任意时刻、在程序中的不同区域分别执行。默认情况下，进程在一个独立的内存空间内运行。这样就可以确保一个进程不会影响其他进程的数据。因此，一个错误的指针访问将引发越界访问异常，或者很容易在特定的进程中找到Bug。不过，数据传递只能通过进程间消息的发送和接收来完成。

一般说来，线程模型较适合于OpenMP，而进程模型较适合于MPI。**在GPU的环境下，就需要将它们混合在一起。CUDA使用一个线程块(block)构成的网格(grid)。这可以看成是一个进程(即线程块)组成的队列(即网格)，而进程间没有通信。每一个线程块内部有很多线程，这些线程以批处理的方式运行，称为线程束(warp)。**

### 并发

如果求解问题的算法在计算每一个点的值的时候，必须知道与其相邻的其他点的值，那么算法的`加速比(speedup)`最终将很难提高。在这种情况下，对一个点的计算就需要投人多个处理器。在这一点上，计算将会变得很慢，因为处理器(或者线程)需要花费更多的时间来进行通信以实现数据共享，而不做任何有意义的计算。CUDA将问题分解成线程块的网格，每块包含多个线程。块可以按任意顺序执行。不过在某个时间点上，只有一部分块处于执行中。一旦被调度到GPU包含的N个`流处理器簇(Streaming Multiprocessors, SM)`中的一个上执行，一个块必须从开始执行到结束。

网格中的块可以被分配到任意一个有空闲槽的SM上。起初，可以采用`轮询调度(round-robin)`策略，以确保分配到每个SM上的块数基本相同。对绝大多数内核程序而言，分块的数量应该是GPU中物理SM数量的八倍或者更多倍。

### 局部性

在现代计算机设计中，这个问题是通过使用`多级缓存`来解决的。缓存的工作基础是`空间局部性(地址空间相对簇集)`和`时间局部性(访问时间的簇集)`。因此，之前被访问过的数据，很可能还要被再次访问(时间局部性)；刚刚被访问过的数据附近的数据很可能马上就会被访问(空间局部性)。当任务被多次重复执行时，缓存的作用会充分地发挥。假设一个工人带着一个装有4件工具的工具箱(缓存)，当分配给他的大量工作都是相同的时候，这4件工具将被反复使用(缓存命中)。

**对于GPU程序设计，程序员必须处理局部性。对于一个给定的工作，他需要事先思考需要哪些工具或零件(即存储地址或据结构)，然后一次性地把它们从硬件仓库(全局内存)取来，在工作开始时就把它们放在正确的工具柜(片内存储器)里。一旦数据被取来，就尽可能把与这些数据相关的不同工作都执行了，避免发生“取来—存回—为了下一个工作再取”。**这个简单的计划使得程序员能够在需要数据前就把它们装人片内存储器。这项工作既适合于诸如GPU内共享内存的显式局部存储模型，也适合于基于CPU的缓存。在共享内存的情况下，你可以通知存储管理单元去取所需的数据，然后就回来处理关于已有的其他数据的实际工作。在缓存的情况下，你可以用特殊的缓存指令来把你认为程序将要使用的数据，先行装入缓存。

与共享内存相比，缓存的麻烦是替换和对“脏”数据的处理。所谓“脏”数据是指缓存中被程序写过的数据。为了获得缓存空间以接纳新的有用数据，“脏”数据必须在新数据装人之前写回到全局内存。这就意味着，对于延迟未知的全局内存访问，我们需要做两次，而不是一次——第一次是写旧的数据，第二次是取新的数据。

### 并行处理的类型

#### 基于任务

就并行程序设计而言，这可以通过编写一个程序来实现，这个程序由多个段组成，这些段将信息从一个应用程序“传递”(通过发送消息)到另一个应用。Linux操作系统的管道操作符(I)就具有这个功能。一个程序(例如grep)的输出是另一个程序(例如sort)的输入。这样，就可以轻松地对一组输人文件进行扫描(通过grep程序)，以查看是否包含特定的字符，然后输出排好序的结果(通过sort程序)。每个程序都分别调度到不同CPU核上运行。一个程序的输出作为下一个程序的输人，这种并行处理称为`流水线并行处理(pipeline parallelism)`。借助一组不同的程序模块，例如，Linux操作系统中各种基于文本的软件工具，用户就可以实现很多有用的功能。也许程序员并不知道每个人所需要的输出，但通过共同工作并可以轻松连接在一起的模块，程序员可以为广泛的、各种类型的用户服务。这种并行处理向着`粗粒度并行处理(coarse-grained parallelism)`发展，即引入许多计算能力很强的处理器，让每个处理器完成一个庞大的任务。

和任何生产流水线一样，基于流水线的并行处理模式的一个问题就是，它的运行速度等于其中最慢的部件。因此，若流水线包含5个部件，每个部件需要工作1秒，那么我们每1秒钟就可以产生一个结果。然而，若有一个部件需要工作2秒，则整个流水线的吞吐率就降至每2秒钟产生一个结果。解决这个问题的方法是`加倍(twofold)`。

`最大加速比`等于程序执行时间最长那部分占整个程序比例的倒数。这称为`阿姆达尔法则(Amdahl'slaw)`，它表示任意加速比的上限。它让我们在一开始，一行代码都还没写的情况下，就知道可能达到的最大加速比。无论如何，你都要做串行操作。即便把所有的计算都移到GPU上，你也需要用CPU来访问辅存、装人和存回数据。你还需要与GPU交换数据以完成输入及输出(1/O)。因此，**最大加速比取决于程序中计算或算术部分占整个程序的比例加上剩下的串行部分的比例**。

#### 基于数据

基于数据的分解则是将第1个数组分成4数据块，CPU中的1个核或者GPU中的1个SM分别处理数组中的1数据块。处理完毕后，再按照相同的方式处理剩下的3个数组。对采用GPU处理而言，将产生4个内核程序，每个内核程序包含4个或者更多的数组块。对问题的这种并行分解是在考虑数据后再考虑变换的思路下进行的。如果算法允许，我们还可以探讨另一种类型的数据并行处理——`单指令多数据(Single Instruction, Multiple Data,SIMD)`模型。这需要特殊的SIMD指令，例如，许多基于x86型CPU提供的MMX、SSE、AVX等。这样，线程0就可以取出多个相邻的数组元素，并用一条SIMD指令来进行处理。用GPU来处理，每个数组则需要进行不同的变换，且每一个变换将映射以被看成一个单独的GPU内核程序。与CPU核不同，每个SM可以处理多个数据块，每个数据块的处理被分成多线程来执行。因此为了提高GPU的使用效率，我们需要对问题进行进一步的分解。通常，我们将块和线程做一个组合，使得一个线程处理一个数组元素。使用CPU，让每个线程处理多个数据，会有很多好处。相比之下，由于GPU仅支持`“加载”(load)/“存储”(store)/“移动”(move)`三条显式的SIMD原语，那么它的应用受到限制。但这反过来促进了GPU`指令级并行处理(Instruction-Level Parallelism, ILP)`功能的增强。

GPU与CPU在缓存上的一个重要差别就是`缓存一致性(cachecoherency)`问题。对于“缓存一致”的系统，一个内存的写操作需要通知所有核的各个级别的缓存。因此，无论何时，所有处理器核看到的内存视图是完全一样的。随着处理器中核数量的增多，这个“通知”的开销迅速增大，**使得“缓存一致性”成为限制一个处理器中核数不能太多的一个重要因素。**缓存一致系统中最坏的情况是，一个内存写操作会强迫每个核的缓存都进行更新，进而每个核都要对相邻的内存单元进行写操作。

### 弗林分类法

它来源于划分不同计算机架构的弗林分类法(Flynn’staxonomy)。根据弗林分类法，计算机的结构类型有:

- SIMD 单指令，多数据
- MIMD 多指令，多数据
- SISD 单指令，单数据
- MISD 多指令，单数据

绝大多数人熟悉的标准串行程序设计遵循的是SISD模型，即在任何时间点上只有一个指令流在处理一个数据项，这相当于一个单核CPU在一个时刻只能执行一个任务。当然，它也可以通过所谓的“分时”(time-slicing)机制，即在多个任务间迅速切换，达到“同时”执行多个任务的效果。

MIMD系统。它具有一个线程或进程的工作池，操作系统负责逐个取出线程或进程，将他们分配到N个CPU核中的一个上执行。每个线程或进程具有一个独立的指令流，CPU内部包含了对不同指令流同时进行解码所需的全部控制逻辑。

SIMD系统尽可能简化了它的实现方法，针对数据并行模型，在任何时间点，只有一个指令流。这样，在CPU内部就只需要一套逻辑来对这个指令流进行解码和执行，而无须多个指令解码通路。由于从芯片内部移除了部分硅实体，因此相比它的MIIMD兄弟，SIMD系统就可以做得更小、更便宜、能耗更低，并能够在更高的时钟频率下工作。

与SIMD稍有不同，GPU实现的是被英伟达称“单指令多线程”(Single Instruction Multiple Thread,SIMT)的模型。在这种模型中，SIMD指令的操作码跟CPU中硬件实现的方式不同，它指示的并不是一个固定的功能。程序员需要通过一个内核程序，指定每个线程的工作内容。因此，内核程序将统一读人数据，程序代码根据需要执行A、B或C变换。实际上，A、B、C是通过重复指令流而顺序执行的，只不过每次执行时屏蔽掉无须参与的线程。与仅支持SIMD的模型相比，从理论上说，这个模型更容易掌握。

### 常用的并行模式

#### 基于循环

基于循环的迭代是实现并行化的模式中最容易的一个。如果循环间的依赖被消除掉了，那么剩下的问题就是在可用的处理器上如何划分工作。划分的原则是让处理器间通信量尽可能少，片内资源(GPU上的寄存器和共享内存，CPU上的一级/二级/三级缓存)的利用率尽可能高。糟糕的是，通信开销通常会随着分块数目的增多而迅速增大，成为提高性能的瓶颈、系统设计的败笔。

对问题的宏观分解应该依据可用的逻辑处理单元的数量。对于CPU，就是可用的逻辑硬件线程的数量；对于GPU，就是流处理器簇(SM)的数量乘以每个SM的最大工作负载。依赖于资源利用率、最大工作负荷和GPU模型，SM的最大工作负载取值范围是1~16块。请注意，我们使用的词是逻辑硬件线程而不是物理硬件线程。某些英特尔CPU采用所谓的“超线程”技术，在一个物理CPU核上支持多个逻辑线程。由于GPU在一个SM内运行多个线程块，所以我们需要用SM的数量乘以每个SM支持的最大块数。

在一个物理设备上支持多个线程可以使设备的吞吐率最大化，也就是说在某线程等待访存或者1/O类型的操作时，设备可以处理其他线程的工作。这个倍数的选择有助于在GPU上实现`负载平衡(load balancing)`，并可以应用于改进新一代GPU然而对于CPU，过多的线程数量却可能会导致性能下降，这主要是由于上下文切换时，操作系统以软件的形式来完成。对缓存和内存带宽竞争的增多，也要求降低线程的数量。因此对于一个基于多核CPU的解决方案，通常它划分问题的粒度要远大于面向GPU的划分粒度。如果在GPU上解决同一个问题，你则要对数据进行重新划分，把它们划分成更小的数据块。

#### 派生/汇集

派生/汇聚模式是一个在串行程序设计中常见的模式，该模式中包含多个同步点而且仅有一部分内容是可以并行处理的，即首先运行串行代码，当运行到某一点时会遇到一个并行区，这个并行区内的工作可以按某种方式分布到P个处理器上。这时，程序就“派生”(fork)出N个线程或进程来并行地完成这些工作。N个线程或进程的执行是独立的、互不相关的，当其工作完成后，则“汇聚”(join)起来。在OpenMP中常常可以看见这种处理方法—程序员用编译指令语句定义可并行区，并行区中的代码被分成N个线程，随后再汇聚成单个线程。

#### 分条/分块

使用CUDA来解决问题，都要求程序员把问题分成若干个小块，即分条/分块。绝大多数并行处理方法也是以不同的形式来使用“条/块化”的概念。甚至像气候模型这样巨大的超级计算问题也必须分为成千上万个块，每个块送到计算机中的一个处理单元上去处理。这种并行处理方法在可扩展方面具有很大的优势。在很多方面，GPU与集成在单个芯片上的对称多处理器系统非常类似。每个流处理器簇(SM)就是一个自主的处理器，能够同时运行多个线程块，每个线程块通常有256或者512个线程。若干个SM集成在一个GPU上，共享一个公共的全局内存空间。

#### 分而治之

分而治之模式也是一种把大问题分解成的小问题的模式，其中每个小问题都是可控制的。通过把这些小的、单独的计算汇集在一起，使得一个大的问题得到解决。常见的分而治之的算法使用“递归”(recursion)来实现，“快速排序”(quicksort)就是一个典型的例子。该算法反复递归地把数据一分为二，一部分是位于支点(pivotpoint)之上的那些点，另一部分是位于支点之下的那些点。最后，当某部分仅包含两个数据时，则对它们做“比较和交换”处理。

## 3. CUDA硬件概述

GPU硬件结构：

- 内存（全局，常量，共享）
- 流处理器簇
- 流处理器

每个SM都需要访问一个所谓的寄存器文件(register File)，这是一组能够以与SP相同速度工作的存储单元，所以访问这组存储单元几乎不需要任何等待时间。不同型号GPU中，寄存器文件的大小可能是不同的。它用来存储SP上运行的线程内部活跃的寄存器。另外，还有一个只供每个SM内部访问的共享内存(shared memory)，这可以用作“程序可控的”高速缓存。与CPU内部的高速缓存不同，它没有自动完成数据替换的硬件逻辑—它完全是由程序员控制的。

对于纹理内存(texture memory)、常量内存(constant memory)和全局内存(global memory)，每一个SM都分别设置有独立访问它们的总线。其中，纹理内存是针对全局内存的一个特殊视图，用来存储插值(interpolation)计算所需的数据，例如，显示2D或3D图像时需要的查找表。它拥有基于硬件进行插值的特性。常量内存用于存储那些只读的数据，所有的GPU卡均对其进行缓存。与纹理内存一样，常量内存也是全局内存建立的一个视图。

图形卡通过GDDR(Graphic Double Data Rate)接口访问全局内存。GDDR是DDR(Double Data Rate)内存的一个高速版本，其内存总线宽度最大可达512位，提供的带宽是CPU对应带宽的5~10倍，在费米架构GPU中最高可达190GB/s。

每个SM还有两个甚至更多的专用单元(Special-PurposeUnit, SPU),SPU专门执行诸如高速的24位正弦函数/余弦函数/指数函数操作等类似的特殊硬件指令。在GT200和费米架构GPU上还设置有双精度浮点运算单元。

## 5. 线程网络、线程块和线程

英伟达为它的硬件调度方式选择了一种比较有趣的模型，即SPMD(单程序多数据，Single Program,Multiple Data)，属于SIMD(单指令多数据)的一种变体。从某些方面来说，这种调度方式的选择是基于英伟达自身底层硬件的实现。并行编程的核心是线程的概念，一个线程就是程序中的一个单一的执行流，就像一件衣服上的一块棉，一块块棉交织在一起织成了衣服，同样，一个个线程组合在一起就形成了并行程序。CUDA的编程模型将线程组合在一起形成了线程束、线程块以及线程网格。

### 线程

线程是并行程序的基本构建块。

#### 问题分解

CPU领域的并行化是向着一个CPU上执行不止一个(单一线程)程序的方向发展。但.这只是我们之前所提到的任务级的并行。一般而言，程序拥有比较密集的数据集，例如，视频编码。对这种程序我们可以采用数据并行模型，将任务分解成N个部分，每个部分单独处理，其中，N代表可供使用的CPU核数。例如，你可以让每个CPU核计算一帧的数据，帧与帧之间没有相互的关联。又或者，你可以选择将每一帧分成N个片段，将每个片段的计算分配到每个独立的核中。

并行也有粗粒度的和细粒度的并行。然而，只有在那些支持大量线程的设备上才能真正实现细粒度的并行，例如，GPU。相比之下，CPU同样支持线程，但伴随着大量的开销，因此它只适合解决粗粒度的并行问题。CPU与GPU不同，它遵从多指令多数据(MIMD)模型，即它可以支持多个独立的指令流。这是一种更加灵活的方式，但由于这种方式是获取多个独立的指令流，而不是平摊多个处理器的单指令流，因此它会带来额外的开销。在此背景下，让我们来考虑用一个图像校正函数来增强数码照片的亮度。如果在GPU上，你可能会为照片上的每个像素点分配一个线程。但如果是在一个四核的CPU上，你可能会为每个CPU核分配照片的1/4图像的数据进行处理。

#### GPU和CPU区别

GPU和CPU设备的架构是迥异的。CPU的设计是用来运行少量比较复杂的任务。GPU的设计则是用来运行大量比较简单的任务。CPU的设计主要是针对执行大量离散而不相关任务的系统。而GPU的设计主要是针对解决那些可以分解成成千上万个小块并可独立运行的问题。因此，CPU适合运行操作系统和应用程序软件，即便有大量的各种各样的任务，它也能够在任何时刻妥善处理。

CPU与GPU支持线程的方式不同。CPU的每个核只有少量的寄存器，每个寄存器都将在执行任何已分配的任务中被用到。为了能执行不同的任务，CPU将在任务与任务之间进行快速的上下文切换。从时间的角度来看，CPU上下文切换的代价是非常昂贵的，因为每一次上下文切换都要将寄存器组里的数据保存到RAM中，等到重新执行这个任务时，又从RAM中恢复。相比之下，GPU同样用到上下文切换这个概念，但它拥有多个寄存器组而不是单个寄存器组。因此，一次上下文切换只需要设置一个寄存器组调度者，用于将当前寄存器组里的内容换进、换出，它的速度比将数据保存到RAM中要快好几个数量级。

CPU和GPU都需要处理失速状态。这种现象通常是由I/O操作和内存获取引起的。CPU在上下文切换的时候会出现这种现象。假定此时有足够多的任务，线程的运行时间也较长，那么它将正常地运转。但如果没有足够多的程序使CPU处于忙碌状态，它就会闲置。如果此时有很多小任务，每一个都会在一小段时间后阻塞，那么CPU将花费大量的时间在上下文切换上，而只有少部分时间在做有用的工作。CPU的调度策略是基于时间分片，将时间平均分配给每个线程。一旦线程的数量增加，上下文切换的时间百分比就会增加，那么效率就会急剧的下降。

GPU就是专门设计用来处理这种失速状态，并且预计这种现象会经常发生。GPU采用的是数据并行的模式，它需要成千上万的线程，从而实现高效的工作。它利用有效的工作池来保证一直有事可做，不会出现闲置状态。因此，当GPU遇到内存获取操作或在等待计算结果时，流处理器就会切换到另一个指令流，而在之后再执行之前被阻塞的指令。

CPU和GPU的一个主要差别就是每台设备上处理器数量的巨大差异。CPU是典型的双核或者四核设备。也就是说它有一定数量的执行核可供程序运行。而目前费米架构的GPU拥有16个SM(流多处理器)，每个SM可看作是CPU的一个核。CPU通常运行的是单线程的程序，即它的每个核的每次迭代仅计算一个数据。然而，GPU默认就是并行的模式，它的SM每次可同时计算32个数而不是像CPU那样只计算一个数，因此，相对于一个四核的CPU来说，GPU的核数目就是其4倍，数据的吞吐量则是其32倍。当然，你可能会说CPU也可以使用所有的可供使用的计算核，以及像MIMX、SSE和AVX那样的指令扩展集，但问题是又有多少CPU程序使用了这种扩展集呢。

GPU为每个SM提供了唯一并且高速的存储器，即共享内存。从某些方面来说，共享内存使用了连接机和cell处理器的设计原理，它为设备提供了在标准寄存器文件之外的本地工作区。自此，程序员可以安心地将数据留在内存中，不必担心由于上下文切换操作需要将数据移出去。另外，共享内存也为线程之间的通讯提供了重要机制。

#### 任务执行模式

任务执行的模式主要有两种。一种基于`锁步(lock-step)`思想，执行N个SP(流处理器)组，每个SP都执行数据不同的相同程序。另一种则是利用巨大的寄存器文件，使线程的切换高效并且达到零负载。GPU能支持大量的线程就是按照这种方式设计的。

所谓的锁步原则到底是什么？指令队列中的每条指令都会分配到SM的每个SP中。每个SM就相当于嵌入了N个计算核心(SP)的处理器。

传统的CPU会将一个单独的指令流分配到每个CPU核心中，而GPU所用的SPMD模式是将同一条指令送到N个逻辑执行单元，也就是说GPU只需要相对于传统的处理器1/N的指令内存带宽。这与许多高端的超级计算机中的向量处理器或单指令多数据处理器很相似。

#### GPU线程

```c
void some_func(){
    int i;
    for (i = 0; i < 128; i++) {
        a[i] = b[i] * c[i];
    }
}
```

这段代码很简单。它让数组b和数组c中下标相同的元素进行相乘，然后将所得的结果保存到相同下标的数组a中。串行代码需要128次for循环(从0~127)。而在CUDA中，我们可以将这段代码直接转换成用128个线程，每个线程都执行下面这段代码:

```c
a[i] = b[i] * c[i];
```

由于循环中每一轮计算与下一轮计算之间没有依赖，因此将这段代码转换成并行程序非常简单。这种并行转换叫做循环并行化。这种并行化是另一种流行的并行语言扩展OpenMP的基础。在一个四核的CPU上，你可以将此计算任务平均分成四部分，让CPU的第一个核计算数组下标为0~31的元素，第二个核计算下标为32~63的元素，第三个核计算下标为64~95的元素，第四个核计算下标为96~127的元素。有些编译器自动就可以做这样的并行划分，而有些则需要程序员在程序中指出哪些循环需要并行。其中，Intel的编译器就非常擅长此道。

在CUDA中，你可以通过创建一个内核函数的方式将循环并行化。所谓的内核函数，就是一个只能在GPU上执行而不能直接在CPU上执行的函数。按照CUDA的编程模式，CPU将主要处理它所擅长的串行代码。当遇到密集计算的代码块时，CPU则将任务交给GPU，让GPU利用它超强的计算能力来完成密集计算。

从概念上看，GPU的内核函数和循环体是一样，只不过将循环的结构移除了。下面这段代码就是一个内核函数：

```c
__global__ void some_func(int *const a, const int * const b, constt int * const c) {
    a[i] = b[i] * c[i];
}
```

仔细观察你会发现循环结构没有了，循环控制变量i也没有了。除此之外，在C的函数前面还多了一个`__global__`的前缀。`__global__`前缀是告诉编译器在编译这个函效的时候生成的是GPU代码而不是CPU代码，并且这段GPU代码在CPU上是全局可见的。

##### 内存空间

CPU和GPU有各自独立的内存空间，因此在GPU代码中，不可以直接访问CPU端的参数，反过来在CPU代码中，也不可以直接访问GPU端的参数。因此，我们之前申明的全局数组a，b，c全是在CPU端的内存中，GPU端的代码是无法直接访问的，所以我们必须在GPU端的内存中也声明这几个数组，然后将数据从CPU端复制到GPU端，以GPU内存指针的方式传递给GPU的内存空间进行读写操作，在计算完毕之后，再将计算的结果复制回CPU端。

##### 线程ID

下一个问题是，i不再是循环控制变量，而是用来标识当前所运行的线程的一个变量。在此，我们将以线程的形式创建128个该函数的实例，而CUDA则提供了一个特殊的变量，它在每个线程中的值都不一样，使得它可以标识每一个线程。这就是线程的索引，即线程ID。我们可以直接将这个线程标号用作数组的下标对数组进行访问。这和MPI中获取程序优先级很相似。

```c
__global__ void some_func(int *const a, const int * const b, constt int * const c) {
    const unsigned int thread_idx = threadIdx.x;
    a[thread_idx] = b[thread_idx] * c[thread_idx];
}
```

线程0中的thread_idx值为0，线程1的为1，依此类推，线程127中的thread_idx值为127。每个线程都进行了两次读内存操作，一次乘法操作，一次存储操作，然后结束。我们注意到，每个线程执行的代码是一样的，但是数据却不相同。这就是CUDA的核心—SPMD模型。

#### 硬件实现

现在我们知道每个SM中有N个核，那么我们该如何运行128个线程?与CPU很相似，GPU的每个线程组被送到SM中，然后N个SP开始执行代码。在得到每个线程的标号之后的第一件事就是从数组b和数组c中各取一个数然后进行乘法运算。不幸的是，这不是立即发生的。实际上，当从存储子系统取得所需要的数之后，已经过去了400~600个GPU时钟周期。在这期间，这一组中的N个线程都将挂起。事实上，线程都是以每32个一组，当所有32个线程都在等待诸如内存读取这样的操作时，它们就会被挂起。术语上，这些线程组叫做线程束(32个线程)或半个线程束(16个线程)。

因此，我们可以将这128个线程分成4组，每组32个线程。

- 首先让所有的线程提取线程标号，计算得到数组地址，然后发出一条内存获取的指令。接着下一条指令是做乘法，但这必须是在从内存读取数据之后。由于读取内存的时间很长，因此线程会挂起。当这组中的32个线程全部挂起，硬件就会切换到另一个线程束。
- 在完成内存读取之后，这些线程将再次置成就绪状态，当再次遇到阻塞操作时，例如另一个线程束进行内存读取，GPU可能将这个线程束用作另一块内存的读取。
- 当所有的线程束(每组32个线程)都在等待内存读取操作完成时，GPU将会闲置。但到达某个时间点之后，GPU将从存储子系统返回一个内存块序列，并且这个序列的顺序通常与发出请求的顺序是一致的。
- 与发出请求的顺序是一致的。假设数组下标为0~31的元素在同一时间返回，线程束0进入就绪队列。如果当前没有任何线程束正在执行，则线程束0将自动进入执行状态。渐渐地其他所有挂起的线程束也都完成了内存读取操作，紧接着它们也会返回到就绪队列。
- 一旦线程束。的乘法指令执行完毕，它就只剩下一条指令需要执行，即将计算得到的结果写人相同下标的数组a中。
- 当所有的线程束都消亡之后，整个内核函数也就结束了，最终将控制返回到CPU端。

#### CUDA内核

CUDA专门定义了一个C语言的扩展用以调用内核。牢记，一个内核仅仅是一个运行在GPU上的函数。调用内核时必须按照以下语法：

```c
kernel_function<<<num_blocks, num_threads>>>(param1, param2, ...)
```

一个内核函数中可以传递很多参数。

两个比较重要的参数，`num_blocks`与`num_threads`。它们可以是实参也可以是形参。在这里，建议使用变量，因为在之后进行性能调优时用起来更加方便。

参数`num_threads`表示执行内核函数的线程数量。在这个例子中，**线程数目即循环迭代的次数**。然而，由于受到硬件的限制，早期的一些设备在一个线程块中最多支持512个线程，而在后期出现的一些设备中则最多可支持1024个线程。

内核调用的下一部分是参数的传递。我们可以通过寄存器或常量内存来进行参数传递，而具体是哪一种方式则视编译器而定。如果使用寄存器传参，每个线程用一个寄存器来传递一个参数。如果现在有128个线程，每个线程传递3个参数，那么就需要3×128=384个寄存器。在每个SM(流处理器)中至少有8192个寄存器，而且随着后续硬件的发展可能会更多。因此，如果在一个SM上只运行一个线程块，每个线程块中只有128个线程，那么每个线程就可以使用64个寄存器(8192个寄存器+128个线程)。

尽管每个线程能使用64个寄存器，但一个SM上只运行一个含有128个线程的线程块并不是一个好方案。只要我们访问内存，SM就会闲置。因此，只有在很少数的情况下，运算强度很强的时候，我们才会考虑选择这种用64个寄存器进行计算的方案。在实际的编程过程中，我们都会尽量避免SM闲置状态的出现。

### 线程块

如果我们将`num_blocks`参数从1修改成2，那么就是告诉GPU硬件，我们将启动两倍于之前线程数量的线程，例如:

```c
some_kernel_func<<<2, 128>>>(a, b, c);
```

这将会调用名为`some_kernel_func`的GPU函数共2×128次，每次都是不同的线程。然而，这样做通常会使thread_idx参数的计算变得更加复杂，而`thread_idx`通常又用来表示数组的位置下标。因此，我们之前简单的内核就要稍作调整:

```c
__global__ void some_kernel_func(int * const a, const int * const b, const int * const c)
{
    const unsigned int thread_idx = (blockIdx.x * blockDim.x) + threadIdx.x;
    a[thread_idx] = b[thread_id×] * c[thread_id×];
}
```

为了计算`thread_idx`这个参数，我们必须考虑线程块的数量。对第一个线程块而言，`blockIdx.x`是0，因此`thread_idx`直接就等于之前使用过的`threadIdx.x`，然而，对于第二个线程块而言，它的`blockIdx.x`的值是1，`blockDim.x`表示本例中所要求的每个线程块启动的线程数量，它的值是128，那么对第二个线程块而言，在计算`thread_idx`时，要在`threadIdx.x`的基础上加上一个1x128线程的基地址。

在介绍线程块加一个基地址的时候有一个错误。现在我们一共启动了256个线程，数组的下标是0~255，如果不更改数组的大小，那么第128个元素~256个元素，将会出现元素访问和写人越界的问题。这种数组越界错误是不会被编译器发现的，程序代码也会根据数组a边界之外的内容来正常执行，因此在调用内核函数的时候要尽量小心，避免这种内存越界访问错误。

#### 线程块的分配

```c
#include <stdio.h>
#include <stdlib.h>

__global__ void what_is_my_id(unsigned int * const block, unsigned int * const thread, unsigned int * const warp, unsigned int * const calc_thread) {
    const unsigned int thread_idx = (blockIdx.x * blockDim.x) + threadIdx.x;
    block[thread_idx] = blockIdx.x;
    thread[thread_idx] = threadIdx.x;

    /* Calculate warp using built in variable warpsize. */
    warp[thread_idx] = threadIdx.x / warpSize;
    calc_thread[thread_idx] = thread_idx;
}

#define ARRAY_SIZE 128
#define ARRAY_SIZE_IN_BYTES (sizeof(unsigned int) * ARRAY_SIZE)

unsigned int cpu_block[ARRAY_SIZE];
unsigned int cpu_thread[ARRAY_SIZE];
unsigned int cpu_warp[ARRAY_SIZE];
unsigned int cpu_calc_thread[ARRAY_SIZE];

int main(void){
    const unsigned int num_blocks = 2;
    const unsigned int num_threads = 64;

    unsigned int * gpu_block;
    unsigned int * gpu_thread;
    unsigned int * gpu_warp;
    unsigned int * gpu_calc_thread;

    unsigned int i;

    /* Allocate four arrays on the GPU */
    cudaMalloc((void **)&gpu_block, ARRAY_SIZE_IN_BYTES);
    cudaMalloc((void **)&gpu_thread, ARRAY_SIZE_IN_BYTES);
    cudaMalloc((void **)&gpu_warp, ARRAY_SIZE_IN_BYTES);
    cudaMalloc((void **)&gpu_calc_thread, ARRAY_SIZE_IN_BYTES);

    what_is_my_id<<<num_blocks, num_threads>>>(gpu_block, gpu_thread, gpu_warp, gpu_calc_thread);

    /* Copy back the GPU results tothe CPU */
    cudaMemcpy(cpu_block, gpu_block,ARRAY_SIZE_IN_BYTES, cudaMemcpyDeviceToHost);
    cudaMemcpy(cpu_thread, gpu_thread,ARRAY_SIZE_IN_BYTES, cudaMemcpyDeviceToHost);
    cudaMemcpy(cpu_warp, gpu_warp,ARRAY_SIZE_IN_BYTES, cudaMemcpyDeviceToHost);
    cudaMemcpy(cpu_calc_thread, gpu_calc_thread,ARRAY_SIZE_IN_BYTES, cudaMemcpyDeviceToHost);

    /* Free the arrays on the GPU as now we're done with them. */
    cudaFree(gpu_block);
    cudaFree(gpu_thread);
    cudaFree(gpu_warp);
    cudaFree(gpu_calc_thread);

    for (i = 0; i < ARRAY_SIZE; i++){
        printf("Calculated Thread: %3u - Block: %3u - Warp: %3u - Thread %3u\n", cpu_calc_thread[i], cpu_block[i], cpu_warp[i], cpu_thread[i]);
    }

    return 0;
}
```

正如我们计算的那样，线程索引是0~127。一共有两个线程块，每个线程块包含64个线程，每个线程块内部线程的索引为0~63。一个线程块包含两个线程束。

```bash
Calculated Thread:   0 - Block:   0 - Warp:   0 - Thread   0
Calculated Thread:   1 - Block:   0 - Warp:   0 - Thread   1
Calculated Thread:   2 - Block:   0 - Warp:   0 - Thread   2
Calculated Thread:   3 - Block:   0 - Warp:   0 - Thread   3
Calculated Thread:   4 - Block:   0 - Warp:   0 - Thread   4
...
Calculated Thread:  30 - Block:   0 - Warp:   0 - Thread  30
Calculated Thread:  31 - Block:   0 - Warp:   0 - Thread  31
Calculated Thread:  32 - Block:   0 - Warp:   1 - Thread  32
Calculated Thread:  33 - Block:   0 - Warp:   1 - Thread  33
Calculated Thread:  34 - Block:   0 - Warp:   1 - Thread  34
...
Calculated Thread:  62 - Block:   0 - Warp:   1 - Thread  62
Calculated Thread:  63 - Block:   0 - Warp:   1 - Thread  63
Calculated Thread:  64 - Block:   1 - Warp:   0 - Thread   0
Calculated Thread:  65 - Block:   1 - Warp:   0 - Thread   1
Calculated Thread:  66 - Block:   1 - Warp:   0 - Thread   2
Calculated Thread:  67 - Block:   1 - Warp:   0 - Thread   3
...
Calculated Thread:  94 - Block:   1 - Warp:   0 - Thread  30
Calculated Thread:  95 - Block:   1 - Warp:   0 - Thread  31
Calculated Thread:  96 - Block:   1 - Warp:   1 - Thread  32
Calculated Thread:  97 - Block:   1 - Warp:   1 - Thread  33
Calculated Thread:  98 - Block:   1 - Warp:   1 - Thread  34
Calculated Thread:  99 - Block:   1 - Warp:   1 - Thread  35
Calculated Thread: 100 - Block:   1 - Warp:   1 - Thread  36
...
Calculated Thread: 126 - Block:   1 - Warp:   1 - Thread  62
Calculated Thread: 127 - Block:   1 - Warp:   1 - Thread  63
```

### 线程网格

一个线程网格是由若干线程块组成的，每个线程块是二维的，拥有X轴与Y轴。此时，每次最多能开启YxXxT个线程。为简单起见，我们限制Y轴方向只有一行线程。

以1920x1080图像为例。通常线程块中线程数量最好是一个线程束大小的整数倍，即32的整数倍。由于设备是以整个线程束为单位进行调度，如果我们不把线程块上的线程数目设成32的整数倍，则最后一个线程束中有一部分线程是没有用的，因此我们必须设置一个限制条件进行限制，防止处理的元素超出X轴方向上所规定的范围。如果不这样做，程序的性能将会降低。

为了防止不合理的内存合并，我们要尽量做到内存的分布与线程的分布达到一一映射的关系。如果我们没能做到这点，程序的性能可能会降低5倍或者更多。

在本例中，我们将在每个线程块上开启192个线程。通常，192是我们所考虑的最少的线程数目。每个线程块192个线程，我们很容易算出处理一行图像需要10个线程块(如图5-9所示)。在这里，选择192这个数是因为×轴方向处理的数据大小1920是它的整数倍，192又是线程束大小的整数倍，这使我们的编程变得更加方便。

![](/img/post_pics/cuda/1.jpeg)

在×轴方向的顶部我们可以得到线程的索引，在Y轴方向我们可以得到行号。由于每一行只处理了一行像素，每一行有10个线程块，因此我们需要1080行来处理整张图片，一共1080x10=10800个线程块。按照这种一个线程处理一个像素的方式，每个线程块开启192个线程，我们一共调度了两百多万个线程。

由于数组的存在，数据往往可能不是一维的。这时，我们可以使用二维的线程块。例如，在很多的图像算法中使用了8×8线程块来处理像素。这里使用像素来进行讲解是因为它们形象直观，更加容易让人理解。而现实中，处理的数据并不一定是基于像素的，也有可能是一个像素的红、绿、蓝三种成分。你可以将其看做一个包含X、Y与z轴的空间坐标系下的一个点，也可以用一个二维或者三维的矩阵来存储数据。

#### 跨幅和偏移

为了让C语言中的数组也能很好地进行映射，线程块也可以看作是一个二维的结构。然而，对于一个二维的线程块，我们需要提出一些新的概念。就像数组的索引一样，要为一个二维数组的Y元素进行索引编号，就必须知道数组的宽度，即X元素的数目。

分组，例如1920 = 192 x 10，这里X范围在0-191，Y范围在0-9。192就是宽度跨度，X（列标号）是偏移。

```bash
(array_idx, X, Y)

 0,0,0    1,1,0   2,2,0   3,3,0   4,4,0
 5,0,1    6,1,1   7,2,1   8,3,1   9,4,1
10,0,2   11,1,2  12,2,2  13,3,2  14,4,2
```

不过，在对数组进行布局的时候，有一点需要我们特别注意，那就是数组的宽度值最好是线程束大小的整数倍。如果不是，填补数组，使它能充满最后一个线程束。但是这样做会增加数据集的大小。此外，我们还需要注意对填补单元的处理，它和数组中其他单元的处理是不同的。我们可以在程序的执行流中使用分支结构(例如，使用if语句)，或者也可以在填补单元计算完毕之后再舍弃它们的计算结果。

#### X与Y方向的线程索引

在一个线程块上分布一个二维数组也就意味着需要两个线程索引。

```c
const unsigned int idx = (blockIdx.x * blockDim.x) + threadIdx.x;
const unsigned int idy = (blockIdx.y * blockDim.y) + threadIdx.y;
```

`blockDim.x`和`blockDim.y`由CUDA运行时库所提供，分别表示X轴与Y轴这两个维度上线程块的数目。如果是32x16维的数据，假设一共4个线程块，每个block有128个线程。这四个线程块可以以条纹布的方式布局，也可以按方块一样布局：

```bash
线程0-15，线程块0   | 线程16-31，线程块0
线程32-47，线程块0  | 线程48-63，线程块0
线程64-79，线程块0  | 线程80-95，线程块0
线程96-111，线程块0 | 线程112-127，线程块0
----------------------------------------
线程0-15，线程块1   | 线程16-31，线程块1
线程32-47，线程块1  | 线程48-63，线程块1
线程64-79，线程块1  | 线程80-95，线程块1
线程96-111，线程块1 | 线程112-127，线程块1
----------------------------------------
线程0-15，线程块2   | 线程16-31，线程块2
线程32-47，线程块2  | 线程48-63，线程块2
线程64-79，线程块2  | 线程80-95，线程块2
线程96-111，线程块2 | 线程112-127，线程块2
----------------------------------------
线程0-15，线程块3   | 线程16-31，线程块3
线程32-47，线程块3  | 线程48-63，线程块3
线程64-79，线程块3  | 线程80-95，线程块3
线程96-111，线程块3 | 线程112-127，线程块3
----------------------------------------
```

```bash
线程0-15，   线程块0 | 线程0-15，   线程块1
线程16-31，  线程块0 | 线程16-31，  线程块1
线程32-47，  线程块0 | 线程32-47，  线程块1
线程48-63，  线程块0 | 线程48-63，  线程块1
线程64-79，  线程块0 | 线程64-79，  线程块1
线程64-79，  线程块0 | 线程64-79，  线程块1 
线程80-95，  线程块0 | 线程80-95，  线程块1
线程96-111， 线程块0 | 线程96-111， 线程块1
线程112-127，线程块0 | 线程112-127，线程块1 
-----------------------------------------
线程0-15，   线程块2 | 线程0-15，   线程块3
线程16-31，  线程块2 | 线程16-31，  线程块3
线程32-47，  线程块2 | 线程32-47，  线程块3
线程48-63，  线程块2 | 线程48-63，  线程块3
线程64-79，  线程块2 | 线程64-79，  线程块3
线程64-79，  线程块2 | 线程64-79，  线程块3 
线程80-95，  线程块2 | 线程80-95，  线程块3
线程96-111， 线程块2 | 线程96-111， 线程块3
线程112-127，线程块2 | 线程112-127，线程块3 

```

注意：不能将每个线程块设为只有一列线程，这样会导致内存访问不连续，进而性能下降。

建议选择条纹布布局。这主要有两个原因。第一个原因是同一个线程块中的线程可以通过共享内存进行通信，这是线程协作中一种比较快的方式。第二个原因是在同一个线程束中的线程存储访问合并在一起了，而在当前费米架构的设备中，高速缓冲存储器的大小是128个字节，一次直接访问连续的128字节比两次分别访问64字节要高效得多。**在方块布局中，0~15号线程映射在一个线程块中，它们访问一块内存数据，但与这块内存相连的数据区则是由另一个线程块访问的**。因此，这两块连续的内存数据通过两次存储访问才获得，而在长方形的布局中，这只需要一次存储访问的操作。但如果我们处理的数组更大，例如64x16，那么32个线程就能进行连续存储访问，每次读出128字节的数据，也就不会出现刚刚所说的那种情况了。

我们通过添加以下代码来选择一种布局方式：

```c
// 条纹布
dim3 threads_rect(32, 4);
dim3 blocks_rect(1, 4);
some_kernel_func<<<blocks_rect, threads_rect>>>(a, b, c);

// 方块
dim3 threads_square(16, 8);
dim3 blocks_square(2,2);
some_kernel_func<<<blocks_square, threads_square>>>(a, b, c);
```

不论哪种布局方式，线程块中的线程总数都是相同的(32x4=128，16x8=128)，只是线程块中线程的排布方式有所不同。
dim3 是CUDA 中一个比较特殊的数据结构，我们可以用这个数据结构创建一个二维的线程块与线程网格。例如在长方形布局的方式中，每个线程块的X轴方向上开启了32个线程，Y轴方向上开启了4个线程。在线程网格上，X轴方向上有1个线程块，Y轴方向有4个线程块。

除了线程块，多个块组成了线程网格，grid。因此引入更多的数据结构：

![](/img/post_pics/cuda/2.jpeg)

计算：

```c
const unsigned int idx = (blockIdx.x * blockDim.x) + threadIdx.x;
const unsigned int idy = (blockIdx.y * blockDim.y) + threadIdx.y;
const unsigned int thread_idx = ((gridDim.x * blockDim.x) * idy) + idx;
```

假如按照条纹布布局

```c
blockDim.x = 32;
blockDim.x = 4;
gridDim.x = 1;
gridDim.y = 4;
```

计算`blockIdx.x = 0, blockIdx.y = 2, threadIdx.x = 30, threadIdx.y = 3`，代入，`idx = 30, idy = 11, thread_idx = 1 * 32 * 11 + 30 = 382`.

### 线程束

**线程束是GPU的基本执行单元。**GPU是一组SIMD 向量处理器的集合。每一组线程或每个线程束中的线程同时执行。在理想状态下，获取当前指令只需要一次访存，然后**将指令广播到这个线程束所占用的所有SP中**。这种模式比CPU模式更加高效，CPU是通过获取单独的执行流来支持任务级的并行。在CPU的模式中，由于每个核运行一个独立的任务，所以理论上，我们可以根据核的数目划分存储带宽但指令吞吐量的效率会下降。而事实上，如果程序的数据都能放入缓存，CPU的片上多级缓存可以有效地隐藏由内存读取带来的延迟

当使用GPU进行编程时，必须使用向量类型指令，因为GPU采用的是向量体系结构只有让代码在成千上万个线程上运行才能充分高效利用GPU的资源。当然，你可以写一个单线程的GPU程序，只需要用一个if语句简单地判断一下线程索引是否为0。但这相对于CPU而言，性能非常的低。然而，当在获取一个连续的CPU初始部署工作时这种方法却很有用。这种方法允许我们在将程序并行化之前进行检查，例如，检查复制到GPU或从GPU复制出的数据是否正常运转。当前，GPU上的一个线程束的大小为32，英伟达公司保留着对这个数修改的权利。因此，他们提供了一个固有变量--`wrapSize`，**我们可以通过这个变量来获取当前硬件所支持的线程束大小**。和其他幻数一样，我们不能通过硬编码的方式改变它的大小。然而许多用SSE指令集优化的程序就可以用硬编码的方式来假定SSE的寄存器大小为128位。

#### 分支

**一个线程束是一个单独的执行单元，使用分支(例如，使用if、else、for、while、do、switch 等语句)可以产生不同的执行流。**在CPU上使用分支很复杂，因为它需要根据之前的运行情况来预测下一次执行到底要执行哪一块代码。在CPU上，指令流通常都会被预提取，然后放入CPU的指令管线中。假设预测是准确的，那么CPU就避免了一次失速事件。如果预测错误，CPU则需要重新执行预测指令，然后获取另一个分支的指令，再将其添入管线之中。

相比之下，GPU对分支的处理就没有这么复杂。GPU在执行完分支结构的一个分支后会接着执行另一个分支。对不满足分支条件的线程，GPU在执行这块代码的时候会将它们设置成未激活状态。当这块代码执行完毕之后，GPU继续执行另一个分支，这时，刚刚不满足分支条件的线程如果满足当前的分支条件，那么它们将被激活，然后执行这一段代码。最后，所有的线程聚合，继续向下执行。具体代码如下：

```c
__global__ some_func(void){
    if(some_condition){
        action_a();
    } else {
        action_b();
    }
}
```

当计算出some condition的值之后，就可进入分支代码块中运行，其中至少会代码中，有一个分支会被执行，否则就没有必要在这里演示这个示例程序了。这里我们假设索引为偶数的线程满足条件为真的情况，执行函数action a0，索引为奇数的线程满足条件为假的情况执行函数 action b0。我们可以看到这一个线程束中不同线程的执行情况。

![](/img/post_pics/cuda/3.jpeg)

为简单起见，我们只画出了 32个线程中16个线程的执行情况，至于这么做的原因我们马上就可以看到。在图中，所有标记“+”的线程都将执行条件为真的分支代码块，而标记“-”的线程都将执行条件为假的分支代码块。**由于硬件每次只能为一个线程束获取一条指令，线程束中一半的线程要执行条件为真的代码段，一半线程执行条件为假的代码段，因此，这时有一半的线程会被阻塞，而另一半线程会执行满足条件的那个分支。**如此，**硬件的利用率只达到了50%。**这种情况并不好，这就好像一个双核的 CPU 只有一个核被利用。对待这个问题，很多懒惰的程序员都会选择逃避,不去理它，但这会使我们程序的性能大打折扣。

这里有一个技巧可以用来避免这个问题。**事实上，在指令执行层，硬件的调度是基于半个线程束，而不是整个线程束。**这意味着，**只要我们能将半个线程束中连续的16个线程划分到同一个分支中，那么硬件就能同时执行分支结构的两个不同条件的分支块**，例如，示例程序中if-else的分支结构。这时硬件的利用率就可以达到100%。如果需要让数据进行两种不同类型的处理，那么我们可以将数据以16为分界线进行划分，这样可以提升性能。下面这段代码是根据线程的索引划分分支的：

```c
if((thread_idx % 32) < 16){
    action_a();
} else {
    action_b();
}
```

#### GPU的利用率

**关注线程束的另一个原因就是防止 GPU未被充分利用。**CUDA的模式是用成千上万的线程来隐藏内存操作的延迟(从发出存储请求到完成访存操作所花的时间)。比较典型的，如对全局内存访问的延迟一般是400~600个时钟周期。在这段时间里，GPU会忙于其他任务，而不是空闲地等待访存操作的完成。

当我们为一个 GPU分配一个内核函数，由于不同的硬件有不同的计算能力，**SM一次最多能容纳的线程数也不尽相同，目前，一个SM最多能容纳768~2048个线程。**SM 一次最多能容纳的线程数量主要与底层硬件的计算能力有关，随着未来硬件的改变，这个数值也会有所改变。

通过表5-1我们可以发现，无论计算能力是何级别，**当每个线程块开启256个线程时，设备地利用率都达到了100%。为了最大限度地利用设备，使程序性能得到提升，我们应尽量将每个线程块开启的线程数设为192或256。**此外，数据集也要和线程布局尽量一一对应以达到更高的性能优化。例如，当需要处理三维点数据时，我们就可以考虑每个线程块开启192 个线程的方案。

讨论了**SM一次最多能容纳的线程数，我们也需要讨论一下SM一次最多能容纳的线程块的数目。**SM容纳线程块的数目会受到内核中是否用到同步的影响。而所谓的同步，就是当程序的线程运行到某个点时，运行到该点的线程需要等待其他还未运行到该点的线程，只有当所有的线程都运行到这个点时，程序才能继续往下执行。例如，我们执行一个读内存的操作，在这个读操作完成之后进行线程同步，所有的线程都要进行读操作，但由于不同线程执行有快有慢，有的线程束很快就完成操作，有的则需要更长的时间，执行完的线程需要等待还未完成读操作的线程，**只有当所有的线程都完成读操作，程序才能继续向下执行。**

执行一个线程块的时间(或延迟)是不确定的。从负载平衡的角度来看，这并不是什么好消息，因为我们想让更多的线程运行起来。**如果现在有256个线程，每个线程束包含32个线程，那么在计算能力为2.X的设备上需要8个线程束。**<u>在计算能力为1.X的设备上，一个SM 一次能调度处理 24个线程束(32x24=768个线程)，而在计算能力为2.X的设备上能一次调度处理48个线程束(32x48=1536个线程)。</u>**每个线程块只有当它执行完全部指令时才会从SM撤走**，在计算能力为2.X或更高的设备上，每个线程块最多能支持1024个线程因此，有时会出现这种情况，所有的线程束都闲置地等待一个线程束执行完毕，SM因为这个原因也闲置了，这大大降低了程序的性能。

由此可见，**每个线程块开启的线程数越多，就潜在地增加了等待执行较慢的线程束的可能性。因为当所有的线程没有到达同步点时GPU是无法继续向下执行的。**因此，有时候我们会选择在每个线程块上开启较少的线程，例如，128个线程，以此来减少之前那种等待的可能性。但这样做会严重地影响性能，例如，在费米架构上的设备上采取这种方案会使设备的利用率降低到原来的 2/3。从表5-1计算能力为2.0(费米架构)的设备中可以看出，只有当线程块的线程数量至少达到192时SM才能被充分地利用。

尽管如此，我们也不必过分花费太多精力在线程束的数量上，因为它毕竟只是一种SM上线程总数的体现形式，**每个SM上总共能容纳的线程数以及利用率才是我们最应该关心的部分。**根据SM所容纳线程块数量及每个线程块对应的线程数，分配线程块中的线程数量，以达到最大利用率。

> 线程，最小单位。
>
> 线程块，应当为64的整数倍。线程块上的所有线程执行完毕后才从SM撤走。线程块中的线程也是有上限的。太多线程或者太少线程均会影响GPU的利用率。
>
> 线程束，书中定义为32，为基本执行单元，假设线程块有256个线程，则有8个线程束。这个体现在分支处理上，一次性执行相同指令，硬件上调度是半个线程束为单位。
>
> SM，可以容纳**线程块**的硬件单元，同时调度**线程束**，例如一次调度48个线程束，则一次调度1536个线程。

### 线程块的调度

假设现在有1024个线程块需要调度处理，但只有8个SM去调度它们。在费米架构的硬件上，当每个线程块中的线程数量并不多时，**每个SM一次最多能接受8个线程块。**当每个线程块中线程的数量比较合理时，一个SM一次一般能处理6~8个线程块。现在，**我们将这1024个线程块分配到6个SM中，平均每个SM需要处理170个线程块，还余4个需要处理。**

像之前提到的那样，前1020个线程块被分配到SM中进行处理，那么它们是怎样分配的？硬件也许会分配6个线程块分配到第一个SM，然后紧接着的6个线程块分配到第二个SM，跟着第三个、第四个，依次类推。也有可能是硬件轮流为每个SM分配一个线程块，线程块0被分配到0号SM中，线程块1被分配到1号SM中，线程块2到2号SM中，依此类推。英伟达并没有公布他们到底是使用的哪种方式，但很有可能是后面一种，因为这种方式能让SM达到合理的负载平衡。

**如果现在我们有19个线程块和4个SM，将所有的线程块都分配到一个SM上进行处理，显然不够合理。如果让前3个SM每个处理6个线程块，最后一个SM处理一个线程块那么最后一个线程块就会很快处理完，然后SM会闲置。**这样做设备的利用率会很低。**如果我们用轮流交替的方式将线程块分配到SM中，每个SM处理4个线程块(4x4=16个线程块)。**还剩三个线程块单独用一个SM再次处理，假设每个线程块执行所花费的时间是一样的，那么通过让每个SM能均衡地处理线程块，而不是像之前那样有的SM负载过重有的SM负载过轻，这样我们就可以减少17%的执行时间。

而在实际的程序中，我们往往会用到成千上万个线程块。在线程块调度者为每个SM初始化分配了线程块之后，就会处于闲置状态，直到有线程块执行完毕。当线程块执行完毕之后就会从SM中撤出，并释放其占用的资源。由于线程块都是相同的大小，因此一个线程块从SM中撤出后另一个在等待队列中的线程块就会被调度执行。**所有的线程块的执行顺序是随机、不确定的，因此，当我们在编写一个程序解决一个问题的时候，不要假定线程块的执行顺序，因为线程块根本就不会按照我们所想的顺序去执行。**

当执行到一些相关联的操作时，我们会遇到一个很严重的问题。我们以浮点加法运算作为例子，**而事实上，浮点加法运算并没有太多的相关联性。我们对一个浮点数组做加和运算，数组按照不同顺序做加和操作时得到的结果会不同。这主要是由舍人误差所引起的，而浮点运算必定会产生这种误差。但不论是什么顺序，最终的结果都是正确的。因此，这并不是一个并行执行的问题，而是一个顺序的问题。**因为即便在 CPU 上用一个单线程的程序来运行，我们也会看到相同的问题。无论是在 CPU 还是 GPU上，我们对一串随机的浮点数做加和运算，不论是从头加到尾，还是从尾加到头，我们都会得到不同的结果。甚至可能更糟的是，在 GPU 上，由于线程块的不确定调度，多次对相同的数据进行计算，每次得到的结果都不相同，但所有结果都是正确的。

### Ex.直方图

串行执行代码：

```c
for (unsigned int i = 0; i < max; i++){
    bin[array[i]] ++;
}
```

转换为并行后带来锁的问题，即多个线程的竞争。cuda提供了原子操作。所谓的原子操作，就是当某个线程对某项数据进行修改的时候，其他优先级比较低的线程无法打断它的操作，直至该线程完成对该数据的所有操作。由于 CUDA 采用的是多线程并行模式，如果我们以锁步的方式执行这段伪码就会遇到一个问题。两个或两个以上的线程在步骤3 获取了相同的值，接着进行加 1操作，然后将新的数据再写回内存。

```c
atomicAdd(&value);
```

保证了对value的加法操作是原子性的，在各个线程之间是串行执行的。

考虑基于任务分解和数据分解模式的并行代码。

- 将输入数组的所有元素分配到每一个线程中。但竞争次数会增加，随着输入数组长度的增加。