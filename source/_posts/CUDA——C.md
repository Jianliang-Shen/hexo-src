---
layout: post
title: CUDA C编程
index_img: /img/post_pics/cuda-c/book.jpeg
date: 2024-04-04 13:02:37
tags: 
    - 并行计算
    - GPU
    - CUDA
categories: 
    - CUDA
---

源码：<https://www.wiley.com/en-us/Professional+CUDA+C+Programming-p-9781118739327>

<!-- more -->

## 目录

- [目录](#目录)
- [第1章 基于CUDA的异构并行计算](#第1章-基于cuda的异构并行计算)
  - [1.1 并行计算](#11-并行计算)
    - [1.1.1 串行编程和并行编程](#111-串行编程和并行编程)
    - [1.1.2 并行性](#112-并行性)
    - [1.1.3 计算机架构](#113-计算机架构)
  - [1.2 异构计算](#12-异构计算)
    - [1.2.1 异构架构](#121-异构架构)
    - [1.2.2 异构计算范例](#122-异构计算范例)
    - [1.2.3 CUDA：一种异构计算平台](#123-cuda一种异构计算平台)
  - [1.3 用GPU输出Hello World](#13-用gpu输出hello-world)
  - [1.4 使用CUDA C编程](#14-使用cuda-c编程)
- [第2章 CUDA编程模型](#第2章-cuda编程模型)
  - [2.1 CUDA编程模型概述](#21-cuda编程模型概述)
    - [2.1.1 CUDA编程结构](#211-cuda编程结构)
    - [2.1.2 内存管理](#212-内存管理)
    - [2.1.3 线程管理](#213-线程管理)
    - [2.1.4 启动一个CUDA核函数](#214-启动一个cuda核函数)
    - [2.1.5 编写核函数](#215-编写核函数)
    - [2.1.6 验证核函数](#216-验证核函数)
    - [2.1.7 处理错误](#217-处理错误)
    - [2.1.8 编译和执行](#218-编译和执行)
  - [2.2 给核函数计时](#22-给核函数计时)
    - [2.2.1 用CPU计时器计时](#221-用cpu计时器计时)
    - [2.2.2 用nvprof工具计时](#222-用nvprof工具计时)
  - [2.3 组织并行线程](#23-组织并行线程)
    - [2.3.1 使用块和线程建立矩阵索引](#231-使用块和线程建立矩阵索引)
    - [2.3.2 使用二维网格和二维块对矩阵求和](#232-使用二维网格和二维块对矩阵求和)
    - [2.3.3 使用一维网格和一维块对矩阵求和](#233-使用一维网格和一维块对矩阵求和)
    - [2.3.4 使用二维网格和一维块对矩阵求和](#234-使用二维网格和一维块对矩阵求和)
  - [2.4 设备管理](#24-设备管理)
    - [2.4.1 使用运行时API查询GPU信息](#241-使用运行时api查询gpu信息)
    - [2.4.2 确定最优GPU](#242-确定最优gpu)
    - [2.4.3 使用nvidia-smi查询GPU信息](#243-使用nvidia-smi查询gpu信息)
    - [2.4.4 在运行时设置设备](#244-在运行时设置设备)
- [第3章 CUDA执行模型](#第3章-cuda执行模型)
  - [3.1 CUDA执行模型概述](#31-cuda执行模型概述)
    - [3.1.1 GPU架构概述](#311-gpu架构概述)
    - [3.1.2 Fermi架构](#312-fermi架构)
    - [3.1.3 Kepler架构](#313-kepler架构)
    - [3.1.4 配置文件驱动优化](#314-配置文件驱动优化)
  - [3.2 理解线程束执行的本质](#32-理解线程束执行的本质)
    - [3.2.1 线程束和线程块](#321-线程束和线程块)
    - [3.2.2 线程束分化](#322-线程束分化)
    - [3.2.3 资源分配](#323-资源分配)
    - [3.2.4 延迟隐藏](#324-延迟隐藏)
    - [3.2.5 占用率](#325-占用率)
    - [3.2.6 同步](#326-同步)
    - [3.2.7 可扩展性](#327-可扩展性)
  - [3.3 并行性的表现](#33-并行性的表现)
    - [3.3.1 用nvprof检测活跃的线程束](#331-用nvprof检测活跃的线程束)
    - [3.3.2 用nvprof检测内存操作](#332-用nvprof检测内存操作)
    - [3.3.3 增大并行性](#333-增大并行性)
  - [3.4 避免分支分化](#34-避免分支分化)
    - [3.4.1 并行归约问题](#341-并行归约问题)
    - [3.4.2 并行归约中的分化](#342-并行归约中的分化)
    - [3.4.3 改善并行归约的分化](#343-改善并行归约的分化)
    - [3.4.4 交错配对的归约](#344-交错配对的归约)
  - [3.5 展开循环](#35-展开循环)
    - [3.5.1 展开的归约](#351-展开的归约)
    - [3.5.2 展开线程的归约](#352-展开线程的归约)
    - [3.5.3 完全展开的归约](#353-完全展开的归约)
    - [3.5.4 模板函数的归约](#354-模板函数的归约)
  - [3.6 动态并行](#36-动态并行)
    - [3.6.1 嵌套执行](#361-嵌套执行)
    - [3.6.2 在GPU上嵌套Hello World](#362-在gpu上嵌套hello-world)
    - [3.6.3 嵌套归约](#363-嵌套归约)
  - [3.7 总结](#37-总结)
- [第4章 全局内存](#第4章-全局内存)
  - [4.1 CUDA内存模型概述](#41-cuda内存模型概述)
    - [4.1.1 内存层次结构的优点](#411-内存层次结构的优点)
    - [4.1.2 CUDA内存模型](#412-cuda内存模型)
  - [4.2 内存管理](#42-内存管理)
    - [4.2.1 内存分配和释放](#421-内存分配和释放)
    - [4.2.2 内存传输](#422-内存传输)
    - [4.2.3 固定内存](#423-固定内存)
    - [4.2.4 零拷贝内存](#424-零拷贝内存)
    - [4.2.5 统一虚拟寻址](#425-统一虚拟寻址)
    - [4.2.6 统一内存寻址](#426-统一内存寻址)
  - [4.3 内存访问模式](#43-内存访问模式)
    - [4.3.1 对齐与合并访问](#431-对齐与合并访问)
    - [4.3.2 全局内存读取](#432-全局内存读取)
    - [4.3.3 全局内存写入](#433-全局内存写入)
    - [4.3.4 结构体数组与数组结构体](#434-结构体数组与数组结构体)
    - [4.3.5 性能调整](#435-性能调整)
  - [4.4 核函数可达到的带宽](#44-核函数可达到的带宽)
    - [4.4.1 内存带宽](#441-内存带宽)
    - [4.4.2 矩阵转置问题](#442-矩阵转置问题)
  - [4.5 使用统一内存的矩阵加法](#45-使用统一内存的矩阵加法)
  - [4.6 总结](#46-总结)
- [第5章 共享内存和常量内存](#第5章-共享内存和常量内存)
  - [5.1 CUDA共享内存概述](#51-cuda共享内存概述)
    - [5.1.1 共享内存](#511-共享内存)
    - [5.1.2 共享内存分配](#512-共享内存分配)
    - [5.1.3 共享内存存储体和访问模式](#513-共享内存存储体和访问模式)
    - [5.1.4 配置共享内存量](#514-配置共享内存量)
    - [5.1.5 同步](#515-同步)
  - [5.2 共享内存的数据布局](#52-共享内存的数据布局)
    - [5.2.1 方形共享内存](#521-方形共享内存)
    - [5.2.2 矩形共享内存](#522-矩形共享内存)
  - [5.3 减少全局内存访问](#53-减少全局内存访问)
    - [5.3.1 使用共享内存的并行归约](#531-使用共享内存的并行归约)
    - [5.3.2 使用展开的并行归约](#532-使用展开的并行归约)
    - [5.3.3 使用动态共享内存的并行归约](#533-使用动态共享内存的并行归约)
    - [5.3.4 有效带宽](#534-有效带宽)
  - [5.4 合并的全局内存访问](#54-合并的全局内存访问)
    - [5.4.1 基准转置内核](#541-基准转置内核)
    - [5.4.2 使用共享内存的矩阵转置](#542-使用共享内存的矩阵转置)
    - [5.4.3 使用填充共享内存的矩阵转置](#543-使用填充共享内存的矩阵转置)
    - [5.4.4 使用展开的矩阵转置](#544-使用展开的矩阵转置)
    - [5.4.5 增大并行性](#545-增大并行性)
  - [5.5 常量内存](#55-常量内存)
    - [5.5.1 使用常量内存实现一维模板](#551-使用常量内存实现一维模板)
    - [5.5.2 与只读缓存的比较](#552-与只读缓存的比较)
  - [5.6 线程束洗牌指令](#56-线程束洗牌指令)
    - [5.6.1 线程束洗牌指令的不同形式](#561-线程束洗牌指令的不同形式)
    - [5.6.2 线程束内的共享数据](#562-线程束内的共享数据)
    - [5.6.3 使用线程束洗牌指令的并行归约](#563-使用线程束洗牌指令的并行归约)
  - [5.7 总结](#57-总结)
- [第6章 流和并发](#第6章-流和并发)
  - [6.1 流和事件概述](#61-流和事件概述)
    - [6.1.1 CUDA流](#611-cuda流)
    - [6.1.2 流调度](#612-流调度)
    - [6.1.3 流的优先级](#613-流的优先级)
    - [6.1.4 CUDA事件](#614-cuda事件)
    - [6.1.5 流同步](#615-流同步)
  - [6.2 并发内核执行](#62-并发内核执行)
    - [6.2.1 非空流中的并发内核](#621-非空流中的并发内核)
    - [6.2.2 Fermi GPU上的虚假依赖关系](#622-fermi-gpu上的虚假依赖关系)
    - [6.2.3 使用OpenMP的调度操作](#623-使用openmp的调度操作)
    - [6.2.4 用环境变量调整流行为](#624-用环境变量调整流行为)
    - [6.2.5 GPU资源的并发限制](#625-gpu资源的并发限制)
    - [6.2.6 默认流的阻塞行为](#626-默认流的阻塞行为)
    - [6.2.7 创建流间依赖关系](#627-创建流间依赖关系)
  - [6.3 重叠内核执行和数据传输](#63-重叠内核执行和数据传输)
    - [6.3.1 使用深度优先调度重叠](#631-使用深度优先调度重叠)
    - [6.3.2 使用广度优先调度重叠](#632-使用广度优先调度重叠)
  - [6.4 重叠GPU和CPU执行](#64-重叠gpu和cpu执行)
  - [6.5 流回调](#65-流回调)
  - [6.6 总结](#66-总结)
- [第7章 调整指令级原语](#第7章-调整指令级原语)
  - [7.1 CUDA指令概述](#71-cuda指令概述)
    - [7.1.1 浮点指令](#711-浮点指令)
    - [7.1.2 内部函数和标准函数](#712-内部函数和标准函数)
    - [7.1.3 原子操作指令](#713-原子操作指令)
  - [7.2 程序优化指令](#72-程序优化指令)
    - [7.2.1 单精度与双精度的比较](#721-单精度与双精度的比较)
    - [7.2.2 标准函数与内部函数的比较](#722-标准函数与内部函数的比较)
    - [7.2.3 了解原子指令](#723-了解原子指令)
    - [7.2.4 综合范例](#724-综合范例)
  - [7.3 总结](#73-总结)
- [第8章 GPU加速库和OpenACC](#第8章-gpu加速库和openacc)
  - [8.1 CUDA库概述](#81-cuda库概述)
    - [8.1.1 CUDA库支持的作用域](#811-cuda库支持的作用域)
    - [8.1.2 通用的CUDA库工作流](#812-通用的cuda库工作流)
  - [8.2 cuSPARSE库](#82-cusparse库)
    - [8.2.1 cuSPARSE数据存储格式](#821-cusparse数据存储格式)
    - [8.2.2 用cuSPARSE进行格式转换](#822-用cusparse进行格式转换)
    - [8.2.3 cuSPARSE功能示例](#823-cusparse功能示例)
    - [8.2.4 cuSPARSE发展中的重要主题](#824-cusparse发展中的重要主题)
    - [8.2.5 cuSPARSE小结](#825-cusparse小结)
  - [8.3 cuBLAS库](#83-cublas库)
    - [8.3.1 管理cuBLAS数据](#831-管理cublas数据)
    - [8.3.2 cuBLAS功能示例](#832-cublas功能示例)
    - [8.3.3 cuBLAS发展中的重要主题](#833-cublas发展中的重要主题)
    - [8.3.4 cuBLAS小结](#834-cublas小结)
  - [8.4 cuFFT库](#84-cufft库)
    - [8.4.1 使用cuFFT API](#841-使用cufft-api)
    - [8.4.2 cuFFT功能示例](#842-cufft功能示例)
    - [8.4.3 cuFFT小结](#843-cufft小结)
  - [8.5 cuRAND库 8.5.1 拟随机数或伪随机数的选择](#85-curand库-851-拟随机数或伪随机数的选择)
    - [8.5.2 cuRAND库概述](#852-curand库概述)
    - [8.5.3 cuRAND介绍](#853-curand介绍)
    - [8.5.4 cuRAND发展中的重要主题](#854-curand发展中的重要主题)
  - [8.6 CUDA 6.0中函数库的介绍](#86-cuda-60中函数库的介绍)
    - [8.6.1 Drop-In库](#861-drop-in库)
    - [8.6.2 多GPU库](#862-多gpu库)
  - [8.7 CUDA函数库的性能研究](#87-cuda函数库的性能研究)
    - [8.7.1 cuSPARSE与MKL的比较](#871-cusparse与mkl的比较)
    - [8.7.2 cuBLAS与MKL BLAS的比较](#872-cublas与mkl-blas的比较)
    - [8.7.3 cuFFT与FFTW及MKL的比较](#873-cufft与fftw及mkl的比较)
    - [8.7.4 CUDA库性能小结](#874-cuda库性能小结)
  - [8.8 OpenACC的使用](#88-openacc的使用)
    - [8.8.1 OpenACC计算指令的使用](#881-openacc计算指令的使用)
    - [8.8.2 OpenACC数据指令的使用](#882-openacc数据指令的使用)
    - [8.8.3 OpenACC运行时API](#883-openacc运行时api)
    - [8.8.4 OpenACC和CUDA库的结合](#884-openacc和cuda库的结合)
    - [8.8.5 OpenACC小结](#885-openacc小结)
  - [8.9 总结](#89-总结)
- [第9章 多GPU编程](#第9章-多gpu编程)
  - [9.1 从一个GPU到多GPU](#91-从一个gpu到多gpu)
    - [9.1.1 在多GPU上执行](#911-在多gpu上执行)
    - [9.1.2 点对点通信](#912-点对点通信)
    - [9.1.3 多GPU间的同步](#913-多gpu间的同步)
  - [9.2 多GPU间细分计算](#92-多gpu间细分计算)
    - [9.2.1 在多设备上分配内存](#921-在多设备上分配内存)
    - [9.2.2 单主机线程分配工作](#922-单主机线程分配工作)
    - [9.2.3 编译和执行](#923-编译和执行)
  - [9.3 多GPU上的点对点通信](#93-多gpu上的点对点通信)
    - [9.3.1 实现点对点访问](#931-实现点对点访问)
    - [9.3.2 点对点的内存复制](#932-点对点的内存复制)
    - [9.3.3 统一虚拟寻址的点对点内存访问](#933-统一虚拟寻址的点对点内存访问)
  - [9.4 多GPU上的有限差分](#94-多gpu上的有限差分)
    - [9.4.1 二维波动方程的模板计算](#941-二维波动方程的模板计算)
    - [9.4.2 多GPU程序的典型模式](#942-多gpu程序的典型模式)
    - [9.4.3 多GPU上的二维模板计算](#943-多gpu上的二维模板计算)
    - [9.4.4 重叠计算与通信](#944-重叠计算与通信)
    - [9.4.5 编译和执行](#945-编译和执行)
  - [9.5 跨GPU集群扩展应用程序](#95-跨gpu集群扩展应用程序)
    - [9.5.1 CPU到CPU的数据传输](#951-cpu到cpu的数据传输)
    - [9.5.2 使用传统MPI在GPU和GPU间传输数据](#952-使用传统mpi在gpu和gpu间传输数据)
    - [9.5.3 使用CUDA-aware MPI进行GPU到GPU的数据传输](#953-使用cuda-aware-mpi进行gpu到gpu的数据传输)
    - [9.5.4 使用CUDA-aware MPI进行节点内GPU到GPU的数据传输](#954-使用cuda-aware-mpi进行节点内gpu到gpu的数据传输)
    - [9.5.5 调整消息块大小](#955-调整消息块大小)
    - [9.5.6 使用GPUDirect RDMA技术进行GPU到GPU的数据传输](#956-使用gpudirect-rdma技术进行gpu到gpu的数据传输)
  - [9.6 总结](#96-总结)
- [第10章 程序实现的注意事项](#第10章-程序实现的注意事项)
  - [10.1 CUDA C的开发过程](#101-cuda-c的开发过程)
    - [10.1.1 APOD开发周期](#1011-apod开发周期)
    - [10.1.2 优化因素](#1012-优化因素)
    - [10.1.3 CUDA代码编译](#1013-cuda代码编译)
    - [10.1.4 CUDA错误处理](#1014-cuda错误处理)
  - [10.2 配置文件驱动优化](#102-配置文件驱动优化)
    - [10.2.1 使用nvprof寻找优化因素](#1021-使用nvprof寻找优化因素)
    - [10.2.2 使用nvvp指导优化](#1022-使用nvvp指导优化)
    - [10.2.3 NVIDIA工具扩展](#1023-nvidia工具扩展)
  - [10.3 CUDA调试](#103-cuda调试)
    - [10.3.1 内核调试](#1031-内核调试)
    - [10.3.2 内存调试](#1032-内存调试)
    - [10.3.3 调试小结](#1033-调试小结)
  - [10.4 将C程序移植到CUDA C的案例研究](#104-将c程序移植到cuda-c的案例研究)
    - [10.4.1 评估crypt](#1041-评估crypt)
    - [10.4.2 并行crypt](#1042-并行crypt)
    - [10.4.3 优化crypt](#1043-优化crypt)
    - [10.4.4 部署crypt](#1044-部署crypt)
    - [10.4.5 移植crypt小结](#1045-移植crypt小结)
  - [10.5 总结](#105-总结)

## 第1章 基于CUDA的异构并行计算

高性能计算的关键部分是中央处理单元（CPU），通常被称为计算机的核心。在早期的计算机中，一个芯片上只有一个CPU，这种结构被称为单核处理器。现在，芯片设计的 趋势是将多个核心集成到一个单一的处理器上，以在体系结构级别支持并行性，这种形式通常被称为多核处理器。因此，并行程序设计可以看作是将一个问题的计算分配给可用的核心以实现并行的过程。

### 1.1 并行计算

#### 1.1.1 串行编程和并行编程

一个程序应包含两个基本的组成部分：`指令和数据`。当一个计算问题被划分成许多小的计算单元后，每个计算单元都是一个任务。在一个任务中，单独的指令负责处理输入和调用一个函数并产生输出。当一个指令处理前一个指令产生的数据时，就有了数据相关性的概念。

#### 1.1.2 并行性

在应用程序中有两种基本的并行类型。

- `任务并行`，当许多任务或函数可以独立地、大规模地并行执行时，这就是任务并行。任务并行的重点在于利用多核系统对任务进行分配。
- `数据并行`，当可以同时处理许多数据时，这就是数据并行。数据并行的重点在于利用多核系统对数据进行分配。

数据并行程序设计的第一步是把数据依据线程进行划分，以使每个线程处理一部分数据。通常来说，有两种方法可以对数据进行划分：`块划分（block partitioning）`和`周期划分（cyclic partitioning）`。

- 在块划分中，每个线程作用于一部分数据，通常这些数据具有相同大小。一组连续的数据被分到一个块内。每个数据块以任意次序被安排给一个线程，线程通常在同一时间只处理一个数据块。
- 在周期划分中，每个线程作用于数据的多部分。更少的数据被分到一个块内。相邻的线程处理相邻的数据块，每个线程可以处理多个数据块。为一个待处理的线程选择一个新的块，就意味着要跳过和现有线程一样多的数据块。

#### 1.1.3 计算机架构

`弗林分类法（Flynn’s Taxonomy）`，它根据指令和数据进入CPU的方式，将计算机架构分为4种不同的类型。

- 单指令单数据（SISD），一种串行架构。在这种计算机上只有一个核心。在任何时间点上只有一个指令流在处理一个数据流。
- 单指令多数据（SIMD），一种并行架构类型。在这种计算机上有多个核心。在任何时间点上所有的核心只有一个指令流处理不同的数据流。
- 多指令单数据（MISD），每个核心通过使用多个指令流处理同一个数据流
- 多指令多数据（MIMD），一种并行架构，在这种架构中，多个核心使用多个指令流来异步处理多个数据流，从而实现空间上的并行性。许多MIMD架构还包括SIMD执行的子组件。

架构层次优化方向：

- 降低`延迟`，延迟是一个操作从开始到完成所需要的时间，常用微秒来表示。
- 提高`带宽`，带宽是单位时间内可处理的数据量，通常表示为MB/s或GB/s。
- 提高`吞吐量`，吞吐量是单位时间内成功处理的运算数量，通常表示为gflops（即每秒十亿次的浮点运算数量），特别是在重点使用浮点计算的科学计算领域经常用到。

>延迟用来衡量完成一次操作的时间，而吞吐量用来衡量在给定的单位时间内处理的操作量。

根据内存组织方式进行进一步划分，一般可以分成下面两种类型。

- 分布式内存的多节点系统，多节点系统中，大型计算引擎是由许多网络连接的处理器构成的。每个处理器有自己的本地内存，而且处理器之间可以通过网络进行通信。这种系统常被称作`集群`。
- 共享内存的多处理器系统，多处理器架构的大小通常是从双处理器到几十个或几百个处理器之间。这些处理器要么是与同一个物理内存相关联，要么共用一个低延迟的链路（如PCI-Express或PCIe）。

`众核（many-core）`通常是指有很多核心（几十或几百个）的多核架构。近年来，计算机架构正在从多核转向众核。

><u>GPU代表了一种众核架构，包括：多线程、MIMD（多指令多数据）、SIMD（单指令多数据），以及指令级并行。NVIDIA公司称这种架构为SIMT（单指令多线程）</u>。
>
> - CPU核心比较重，用来处理非常复杂的控制逻辑，以优化串行程序执行。
> - GPU核心较轻，用于优化具有简单控制逻辑的数据并行任务，注重并行程序的吞吐量。

### 1.2 异构计算

CPU和GPU是两个独立的处理器，它们通过单个计算节点中的PCI-Express总线相连。在这种典型的架构中，<u>GPU指的是离散的设备从同构系统到异构系统的转变是高性能计算史上的一个里程碑</u>。同构计算使用的是同一架构下的一个或多个处理器来执行一个应用。而异构计算则使用一个处理器架构来执行一个应用，为任务选择适合它的架构，使其最终对性能有所改进。

#### 1.2.1 异构架构

一个典型的异构计算节点包括两个多核CPU插槽和两个或更多个的众核GPU。GPU不是一个独立运行的平台而是CPU的协处理器。一个异构应用包括两个部分。

- `主机代码`
- `设备代码`

主机代码在CPU上运行，设备代码在GPU上运行。异构平台上执行的应用通常由CPU初始化。在设备端加载计算密集型任务之前，CPU代码负责管理设备端的环境、代码和数据。

以下是描述GPU容量的两个重要特征。

- `CUDA核心数量`
- `内存大小`

有两种不同的指标来评估GPU的性能。

- `峰值计算性能`，峰值计算性能是用来评估计算容量的一个指标，通常定义为每秒能处理的单精度或双精度浮点运算的数量。峰值性能通常用`GFlops（每秒十亿次浮点运算）`或`TFlops（每秒万亿次浮点运算）`来表示。
- `内存带宽`，内存带宽是从内存中读取或写入数据的比率。内存带宽通常用`GB/s`表示。

#### 1.2.2 异构计算范例

GPU计算并不是要取代CPU计算。对于特定的程序来说，每种计算方法都有它自己的优点。CPU计算适合处理控制密集型任务，GPU计算适合处理包含数据并行的计算密集型任务。GPU与CPU结合后，能有效提高大规模计算问题的处理速度与性能。CPU针对动态工作负载进行了优化，这些动态工作负载是由短序列的计算操作和不可预测的控制流程标记的；而GPU在其他领域内的目的是：处理由计算任务主导的且带有简单控制流的工作负载。可以从两个方面来区分CPU和GPU应用的范围：

- 并行级
- 数据规模

<u>如果一个问题有较小的数据规模、复杂的控制逻辑和/或很少的并行性，那么最好选择CPU处理该问题，因为它有处理复杂逻辑和指令级并行性的能力。相反，如果该问题包含较大规模的待处理数据并表现出大量的数据并行性，那么使用GPU是最好的选择。因为GPU中有大量可编程的核心，可以支持大规模多线程运算，而且相比CPU有较大的峰值带宽。</u>

<u>CPU上的线程通常是重量级的实体。操作系统必须交替线程使用启用或关闭CPU执行通道以提供多线程处理功能。上下文的切换缓慢且开销大。GPU上的线程是高度轻量级的。在一个典型的系统中会有成千上万的线程排队等待工作。如果GPU必须等待一组线程执行结束，那么它只要调用另一组线程执行其他任务即可。CPU的核被设计用来尽可能减少一个或两个线程运行时间的延迟，而GPU的核是用来处理大量并发的、轻量级的线程，以最大限度地提高吞吐量。</u>

#### 1.2.3 CUDA：一种异构计算平台

CUDA是一种通用的并行计算平台和编程模型，它利用NVIDIA GPU中的并行计算引擎能更有效地解决复杂的计算问题。通过使用CUDA，你可以像在CPU上那样，通过GPU来进行计算。

CUDA提供了两层API来管理GPU设备和组织线程：

- `CUDA驱动API`，驱动API是一种低级API，它相对来说较难编程，但是它对于在GPU设备使用上提供了更多的控制。
- `CUDA运行时API`，运行时API是一个高级API，它在驱动API的上层实现。每个运行时API函数都被分解为更多传给驱动API的基本运算。

一个CUDA程序包含了以下两个部分的混合：

- `在CPU上运行的主机代码`
- `在GPU上运行的设备代码`

NVIDIA的`CUDA nvcc`编译器在编译过程中将设备代码从主机代码中分离出来，CUDA nvcc编译器是以广泛使用LLVM开源编译系统为基础的。主机代码是标准的C代码，使用C编译器进行编译。设备代码，也就是核函数，是用扩展的带有标记数据并行函数关键字的CUDA C语言编写的。设备代码通过nvcc进行编译。在链接阶段，在内核程序调用和显示GPU设备操作中添加CUDA运行时库。

### 1.3 用GPU输出Hello World

```c
#include "../common/common.h"
#include <stdio.h>

/*
 * A simple introduction to programming in CUDA. This program prints "Hello
 * World from GPU! from 10 CUDA threads running on the GPU.
 */

__global__ void helloFromGPU()
{
    printf("Hello World from GPU!\n");
}

int main(int argc, char **argv)
{
    printf("Hello World from CPU!\n");

    helloFromGPU<<<1, 10>>>();
    CHECK(cudaDeviceReset());
    return 0;
}
```

```txt
nvcc hello.cu - o hello
./hello
Hello World from CPU!
Hello World from GPU!
Hello World from GPU!
Hello World from GPU!
Hello World from GPU!
Hello World from GPU!
Hello World from GPU!
Hello World from GPU!
Hello World from GPU!
Hello World from GPU!
Hello World from GPU!
```

**一个典型的CUDA编程结构包括5个主要步骤。**

1. **分配GPU内存。**
2. **从CPU内存中拷贝数据到GPU内存。**
3. **调用CUDA内核函数来完成程序指定的运算。**
4. **将数据从GPU拷回CPU内存。**
5. **释放GPU内存空间。**

### 1.4 使用CUDA C编程

<u>数据局部性在并行编程中是一个非常重要的概念。数据局部性指的是数据重用，以降低内存访问的延迟。数据局部性有两种基本类型。时间局部性是指在相对较短的时间段内数据和/或资源的重用。空间局部性是指在相对较接近的存储空间内数据元素的重用。现代的CPU架构使用大容量缓存来优化具有良好空间局部性和时间局部性的应用程序。</u>设计高效利用CPU缓存的算法是程序员的工作。程序员必须处理低层的缓存优化，但由于线程在底层架构中的安排是透明的，所以这一点程序员是没有办法优化的。

CUDA中有`内存层次和线程层次`的概念，使用如下结构，有助于你对线程执行进行更高层次的控制和调度：

- **内存层次结构**
- **线程层次结构**

例如，在CUDA编程模型中使用的`共享内存`（一个特殊的内存）。共享内存可以视为一个被软件管理的高速缓存，通过为主内存节省带宽来大幅度提高运行速度。有了共享内存，你可以直接控制代码的数据局部性。

CUDA核中有3个关键抽象：`线程组的层次结构，内存的层次结构以及障碍同步`。这3个抽象是最小的一组语言扩展。

## 第2章 CUDA编程模型

### 2.1 CUDA编程模型概述

CUDA编程模型提供了一个计算机架构抽象作为应用程序和其可用硬件之间的桥梁。CUDA编程模型利用GPU架构的计算能力提供了以下几个特有功能：

- 一种通过层次结构在GPU中组织线程的方法
- 一种通过层次结构在GPU中访问内存的方法

#### 2.1.1 CUDA编程结构

CUDA编程模型使用由C语言扩展生成的注释代码在异构计算系统中执行应用程序。在一个异构环境中包含多个CPU和GPU，每个GPU和CPU的内存都由一条PCI-Express总线分隔开。因此，需要注意区分以下内容：

- 主机：CPU及其内存（主机内存）
- 设备：GPU及其内存（设备内存）

CUDA 6.0之后，NVIDIA提出了名为`统一寻址（Unified Memory）`的编程模型的改进，它连接了主机内存和设备内存空间，可使用单个指针访问CPU和GPU内存，无须彼此之间手动拷贝数据。

`内核（kernel）是CUDA编程模型的一个重要组成部分，其代码在GPU上运行。`多数情况下，主机可以独立地对设备进行操作。内核一旦被启动，管理权立刻返回给主机，释放CPU来执行由设备上运行的并行代码实现的额外的任务。CUDA编程模型主要是异步的，因此在GPU上进行的运算可以与主机-设备通信重叠。一个典型的CUDA程序包括由并行代码互补的串行代码。

#### 2.1.2 内存管理

用于执行GPU内存分配的是`cudaMalloc`函数，其函数原型为：

```c
cudaError_t cudaMalloc(void** devPtr, size_t size)
```

`cudaMemcpy`函数负责主机和设备之间的数据传输，其函数原型为：

```c
cudaError_t cudaMemcmpy(void* dst, const void* src, size_t count, cudaMemcpyKind kind)
```

这个函数以同步方式执行，因为在`cudaMemcpy`函数返回以及传输操作完成之前主机应用程序是阻塞的。除了内核启动之外的CUDA调用都会返回一个错误的枚举类型`cudaError_t`。

CUDA编程模型最显著的一个特点就是揭示了内存层次结构。每一个GPU设备都有用于不同用途的存储类型。在GPU内存层次结构中，最主要的两种内存是全局内存和共享内存。全局类似于CPU的系统内存，而共享内存类似于CPU的缓存。然而GPU的共享内存可以由CUDA C的内核直接控制。

>使用CUDA C进行编程的人最常犯的错误就是对不同内存空间的不恰当引用。对于在GPU上被分配的内存来说，设备指针在主机代码中可能并没有被引用。

#### 2.1.3 线程管理

当核函数在主机端启动时，它的执行会移动到设备上，此时设备中会产生大量的线程并且每个线程都执行由核函数指定的语句。了解如何组织线程是CUDA编程的一个关键部分。由一个内核启动所产生的所有线程统称为一个网格。同一网格中的所有线程共享相同的全局内存空间。一个网格由多个线程块构成，一个线程块包含一组线程，同一线程块内的线程协作可以通过以下方式来实现。

- `同步`
- `共享内存`

不同块内的线程不能协作。线程依靠以下两个坐标变量来区分彼此：

- **blockIdx（线程块在线程格内的索引）**
- **threadIdx（块内的线程索引）**

CUDA可以组织三维的网格和块。网格和块的维度由下列两个内置变量指定。

- **blockDim（线程块的维度，用每个线程块中的线程数来表示）**
- **gridDim（线程格的维度，用每个线程格中的线程数来表示）**

<u>通常，一个线程格会被组织成线程块的二维数组形式，一个线程块会被组织成线程的三维数组形式。</u>线程格和线程块均使用3个`dim3`类型的无符号整型字段，而未使用的字段将被初始化为1且忽略不计。

[checkDimension.cu](https://github.com/Jianliang-Shen/cuda-c-samples/blob/main/chapter02/checkDimension.cu)定义了一个包含3个线程的一维线程块，以及一个基于块和数据大小定义的一定数量线程块的一维线程网格：

```txt
grid.x 2 grid.y 1 grid.z 1
block.x 3 block.y 1 block.z 1
threadIdx:(0, 0, 0)
threadIdx:(1, 0, 0)
threadIdx:(2, 0, 0)
threadIdx:(0, 0, 0)
threadIdx:(1, 0, 0)
threadIdx:(2, 0, 0)
blockIdx:(0, 0, 0)
blockIdx:(0, 0, 0)
blockIdx:(0, 0, 0)
blockIdx:(1, 0, 0)
blockIdx:(1, 0, 0)
blockIdx:(1, 0, 0)
blockDim:(3, 1, 1)
blockDim:(3, 1, 1)
blockDim:(3, 1, 1)
blockDim:(3, 1, 1)
blockDim:(3, 1, 1)
blockDim:(3, 1, 1)
gridDim:(2, 1, 1)
gridDim:(2, 1, 1)
gridDim:(2, 1, 1)
gridDim:(2, 1, 1)
gridDim:(2, 1, 1)
gridDim:(2, 1, 1)
```

对于一个给定的数据大小，确定网格和块尺寸的一般步骤为：

- **确定块的大小**
- **在已知数据大小和块大小的基础上计算网格维度**

要确定块尺寸，通常需要考虑：

- **内核的性能特性**
- **GPU资源的限制**

[defineGridBlock.cu](https://github.com/Jianliang-Shen/cuda-c-samples/blob/main/chapter02/defineGridBlock.cu)使用了一个一维网格和一个一维块来说明当块的大小改变时，网格的尺寸也会随之改变。由于应用程序中的数据大小是固定的，因此当块的大小发生改变时，相应的网格尺寸也会发生改变。

```txt
grid.x 1 block.x 1024 
grid.x 2 block.x 512 
grid.x 4 block.x 256 
grid.x 8 block.x 128
```

CUDA的特点之一就是通过编程模型揭示了一个两层的线程层次结构。由于一个内核启动的网格和块的维数会影响性能，这一结构为程序员优化程序提供了一个额外的途径。网格和块的维度存在几个限制因素，对于块大小的一个主要限制因素就是可利用的计算资源，如寄存器，共享内存等。

#### 2.1.4 启动一个CUDA核函数

CUDA内核调用是对C语言函数调用语句的延伸，`<<<>>>`运算符内是核函数的执行配置。

```c
kernel_name <<<grid, block>>>(args list)
```

通过指定网格和块的维度，你可以进行以下配置：

- **内核中线程的数目**
- **内核中使用的线程布局**

核函数的调用与主机线程是异步的。核函数调用结束后，控制权立刻返回给主机端。你可以调用以下函数来强制主机端程序等待所有的核函数执行结束：

```c
cudaError_t cudaDeviceSynchronize(void);
```

一些CUDA运行时API在主机和设备之间是隐式同步的。当使用`cudaMemcpy`函数在主机和设备之间拷贝数据时，主机端隐式同步，即主机端程序必须等待数据拷贝完成后才能继续执行程序。

#### 2.1.5 编写核函数

核函数是在设备端执行的代码。在核函数中，需要为一个线程规定要进行的计算以及要进行的数据访问。当核函数被调用时，许多不同的CUDA线程并行执行同一个计算任务。核函数必须有一个void返回类型。以下是用`__global__`声明定义核函数：

```c
__global__ void kernel_name(args list);
```

- `__global__`：在设备端执行，可以从主机端或计算能力3.0的设备中调用
- `__device__`：在设备端执行，仅能从设备端调用
- `__host__`：在主机端执行，仅能从主机端调用，可以省略

`__device__`和`__host__`限定符可以一齐使用，这样函数可以同时在主机和设备端进行编译。以下限制适用于所有核函数：

- **只能访问设备内存**
- **必须具有void返回类型**
- **不支持可变数量的参数**
- **不支持静态变量**
- **显示异步行为**

#### 2.1.6 验证核函数

略

#### 2.1.7 处理错误

略

#### 2.1.8 编译和执行

[sumArraysOnGPU-small-case.cu](https://github.com/Jianliang-Shen/cuda-c-samples/blob/main/chapter02/sumArraysOnGPU-small-case.cu)

```txt
./sumArraysOnGPU-small-case Starting...
Vector size 32
Execution configure <<<1, 32>>>
Arrays match.
```

### 2.2 给核函数计时

衡量核函数性能的方法有很多。最简单的方法是**在主机端使用一个CPU或GPU计时器来计算内核的执行时间。**

#### 2.2.1 用CPU计时器计时

[sumArraysOnGPU-timer.cu](https://github.com/Jianliang-Shen/cuda-c-samples/blob/main/chapter02/sumArraysOnGPU-timer.cu)展示了如何在主函数中用CPU计时器测试向量加法的核函数。

```txt
./sumArraysOnGPU-timer Starting...
Using Device 0: Orin
Vector size 16777216
initialData Time elapsed 1.048767 sec
sumArraysOnHost Time elapsed 0.075818 sec
sumArraysOnGPU <<<  32768, 512  >>>  Time elapsed 0.004480 sec
Arrays match.

# sumArraysOnGPU <<<  131072, 128  >>>  Time elapsed 0.004350 sec
# sumArraysOnGPU <<<  65536, 256  >>>  Time elapsed 0.004323 sec
# sumArraysOnGPU <<<  16384, 1024  >>>  Time elapsed 0.006946 sec
```

#### 2.2.2 用nvprof工具计时

Tegra不支持。略

### 2.3 组织并行线程

现在通过一个矩阵加法的例子来进一步说明这一点。对于矩阵运算，传统的方法是在内核中使用一个包含二维网格与二维块的布局来组织线程。但是，这种传统的方法无法获得最佳性能。在矩阵加法中使用以下布局将有助于了解更多关于网格和块的启发性的用法：

- 由二维线程块构成的二维网格
- 由一维线程块构成的一维网格
- 由一维线程块构成的二维网格

#### 2.3.1 使用块和线程建立矩阵索引

[checkThreadIndex.cu](https://github.com/Jianliang-Shen/cuda-c-samples/blob/main/chapter02/checkThreadIndex.cu)检查块和线程索引：

`线程索引，块索引，矩阵坐标，线性全局内存偏移量，相应元素的值`

```txt
./checkThreadIndex Starting...
Using Device 0: Orin

Matrix: (8.6)
  0  1  2  3  4  5  6  7
  8  9 10 11 12 13 14 15
 16 17 18 19 20 21 22 23
 24 25 26 27 28 29 30 31
 32 33 34 35 36 37 38 39
 40 41 42 43 44 45 46 47

thread_id (0,0) block_id (1,1) coordinate (4,2) global index 20 ival 20
thread_id (1,0) block_id (1,1) coordinate (5,2) global index 21 ival 21
thread_id (2,0) block_id (1,1) coordinate (6,2) global index 22 ival 22
thread_id (3,0) block_id (1,1) coordinate (7,2) global index 23 ival 23
thread_id (0,1) block_id (1,1) coordinate (4,3) global index 28 ival 28
thread_id (1,1) block_id (1,1) coordinate (5,3) global index 29 ival 29
thread_id (2,1) block_id (1,1) coordinate (6,3) global index 30 ival 30
thread_id (3,1) block_id (1,1) coordinate (7,3) global index 31 ival 31
thread_id (0,0) block_id (1,0) coordinate (4,0) global index  4 ival  4
thread_id (1,0) block_id (1,0) coordinate (5,0) global index  5 ival  5
thread_id (2,0) block_id (1,0) coordinate (6,0) global index  6 ival  6
thread_id (3,0) block_id (1,0) coordinate (7,0) global index  7 ival  7
thread_id (0,1) block_id (1,0) coordinate (4,1) global index 12 ival 12
thread_id (1,1) block_id (1,0) coordinate (5,1) global index 13 ival 13
thread_id (2,1) block_id (1,0) coordinate (6,1) global index 14 ival 14
thread_id (3,1) block_id (1,0) coordinate (7,1) global index 15 ival 15
thread_id (0,0) block_id (0,1) coordinate (0,2) global index 16 ival 16
thread_id (1,0) block_id (0,1) coordinate (1,2) global index 17 ival 17
thread_id (2,0) block_id (0,1) coordinate (2,2) global index 18 ival 18
thread_id (3,0) block_id (0,1) coordinate (3,2) global index 19 ival 19
thread_id (0,1) block_id (0,1) coordinate (0,3) global index 24 ival 24
thread_id (1,1) block_id (0,1) coordinate (1,3) global index 25 ival 25
thread_id (2,1) block_id (0,1) coordinate (2,3) global index 26 ival 26
thread_id (3,1) block_id (0,1) coordinate (3,3) global index 27 ival 27
thread_id (0,0) block_id (1,2) coordinate (4,4) global index 36 ival 36
thread_id (1,0) block_id (1,2) coordinate (5,4) global index 37 ival 37
thread_id (2,0) block_id (1,2) coordinate (6,4) global index 38 ival 38
thread_id (3,0) block_id (1,2) coordinate (7,4) global index 39 ival 39
thread_id (0,1) block_id (1,2) coordinate (4,5) global index 44 ival 44
thread_id (1,1) block_id (1,2) coordinate (5,5) global index 45 ival 45
thread_id (2,1) block_id (1,2) coordinate (6,5) global index 46 ival 46
thread_id (3,1) block_id (1,2) coordinate (7,5) global index 47 ival 47
thread_id (0,0) block_id (0,0) coordinate (0,0) global index  0 ival  0
thread_id (1,0) block_id (0,0) coordinate (1,0) global index  1 ival  1
thread_id (2,0) block_id (0,0) coordinate (2,0) global index  2 ival  2
thread_id (3,0) block_id (0,0) coordinate (3,0) global index  3 ival  3
thread_id (0,1) block_id (0,0) coordinate (0,1) global index  8 ival  8
thread_id (1,1) block_id (0,0) coordinate (1,1) global index  9 ival  9
thread_id (2,1) block_id (0,0) coordinate (2,1) global index 10 ival 10
thread_id (3,1) block_id (0,0) coordinate (3,1) global index 11 ival 11
thread_id (0,0) block_id (0,2) coordinate (0,4) global index 32 ival 32
thread_id (1,0) block_id (0,2) coordinate (1,4) global index 33 ival 33
thread_id (2,0) block_id (0,2) coordinate (2,4) global index 34 ival 34
thread_id (3,0) block_id (0,2) coordinate (3,4) global index 35 ival 35
thread_id (0,1) block_id (0,2) coordinate (0,5) global index 40 ival 40
thread_id (1,1) block_id (0,2) coordinate (1,5) global index 41 ival 41
thread_id (2,1) block_id (0,2) coordinate (2,5) global index 42 ival 42
thread_id (3,1) block_id (0,2) coordinate (3,5) global index 43 ival 43
```

#### 2.3.2 使用二维网格和二维块对矩阵求和

[sumMatrixOnGPU-2D-grid-2D-block.cu](https://github.com/Jianliang-Shen/cuda-c-samples/blob/main/chapter02/sumMatrixOnGPU-2D-grid-2D-block.cu)

```c
// grid 2D block 2D
__global__ void sumMatrixOnGPU2D(float *MatA, float *MatB, float *MatC, int nx,
                                 int ny)
{
    unsigned int ix = threadIdx.x + blockIdx.x * blockDim.x;
    unsigned int iy = threadIdx.y + blockIdx.y * blockDim.y;
    unsigned int idx = iy * nx + ix;

    if (ix < nx && iy < ny)
        MatC[idx] = MatA[idx] + MatB[idx];
}

// ...

// invoke kernel at host side
int dimx = 32;
int dimy = 32;
dim3 block(dimx, dimy);
dim3 grid((nx + block.x - 1) / block.x, (ny + block.y - 1) / block.y);
```

#### 2.3.3 使用一维网格和一维块对矩阵求和

[sumMatrixOnGPU-1D-grid-1D-block.cu](https://github.com/Jianliang-Shen/cuda-c-samples/blob/main/chapter02/sumMatrixOnGPU-1D-grid-1D-block.cu)

```c
// grid 1D block 1D
__global__ void sumMatrixOnGPU1D(float *MatA, float *MatB, float *MatC, int nx,
                                 int ny)
{
    unsigned int ix = threadIdx.x + blockIdx.x * blockDim.x;

    if (ix < nx )
        for (int iy = 0; iy < ny; iy++)
        {
            int idx = iy * nx + ix;
            MatC[idx] = MatA[idx] + MatB[idx];
        }
}

// ...

// invoke kernel at host side
int dimx = 32;
dim3 block(dimx, 1);
dim3 grid((nx + block.x - 1) / block.x, 1);
```

#### 2.3.4 使用二维网格和一维块对矩阵求和

[sumMatrixOnGPU-2D-grid-1D-block.cu](https://github.com/Jianliang-Shen/cuda-c-samples/blob/main/chapter02/sumMatrixOnGPU-2D-grid-1D-block.cu)

```c
// grid 2D block 1D
__global__ void sumMatrixOnGPUMix(float *MatA, float *MatB, float *MatC, int nx,
                                  int ny)
{
    unsigned int ix = threadIdx.x + blockIdx.x * blockDim.x;
    unsigned int iy = blockIdx.y;
    unsigned int idx = iy * nx + ix;

    if (ix < nx && iy < ny)
        MatC[idx] = MatA[idx] + MatB[idx];
}

// ...

// invoke kernel at host side
int dimx = 32;
dim3 block(dimx, 1);
dim3 grid((nx + block.x - 1) / block.x, ny);
```

从矩阵加法的例子中可以看出：

- **改变执行配置对内核性能有影响**
- **传统的核函数实现一般不能获得最佳性能**
- **对于一个给定的核函数，尝试使用不同的网格和线程块大小可以获得更好的性能**

### 2.4 设备管理

#### 2.4.1 使用运行时API查询GPU信息

可以使用以下函数查询关于GPU设备的所有信息：

```c
cudaError_t cudaGetDeviceProperties(cudaDeviceProp* prop, int device)
```

[checkDeviceInfor.cu](https://github.com/Jianliang-Shen/cuda-c-samples/blob/main/chapter02/checkDeviceInfor.cu)将返回所安装设备的不同信息。

```txt
./checkDeviceInfor Starting...
Detected 1 CUDA Capable device(s)
Device 0: "Orin"
  CUDA Driver Version / Runtime Version          11.4 / 11.4
  CUDA Capability Major/Minor version number:    8.7
  Total amount of global memory:                 7.16 GBytes (7693115392 bytes)
  GPU Clock rate:                                624 MHz (0.62 GHz)
  Memory Clock rate:                             624 Mhz
  Memory Bus Width:                              64-bit
  L2 Cache Size:                                 2097152 bytes
  Max Texture Dimension Size (x,y,z)             1D=(131072), 2D=(131072,65536), 3D=(16384,16384,16384)
  Max Layered Texture Size (dim) x layers        1D=(32768) x 2048, 2D=(32768,32768) x 2048
  Total amount of constant memory:               65536 bytes
  Total amount of shared memory per block:       49152 bytes
  Total number of registers available per block: 65536
  Warp size:                                     32
  Maximum number of threads per multiprocessor:  1536
  Maximum number of threads per block:           1024
  Maximum sizes of each dimension of a block:    1024 x 1024 x 64
  Maximum sizes of each dimension of a grid:     2147483647 x 65535 x 65535
  Maximum memory pitch:                          2147483647 bytes
```

#### 2.4.2 确定最优GPU

一些系统支持多GPU。在每个GPU都不同的情况下，选择性能最好的GPU运行核函数是非常重要的。通过比较GPU包含的多处理器的数量选出计算能力最佳的GPU。

#### 2.4.3 使用nvidia-smi查询GPU信息

Jetson不支持nvidia-smi，略。补充：可以通过Jtop查看。

#### 2.4.4 在运行时设置设备

设置运行时环境变量`CUDA_VISIBLE_DEVICES=2`。nvidia驱动程序会屏蔽其他GPU，这时设备2作为设备0出现在应用程序中。也可以使用`CUDA_VISIBLE_DEVICES`指定多个设备。例如，如果想测试GPU 2和GPU 3，可以设置`CUDA_VISIBLE_DEVICES=2，3`。然后，在运行时，nvidia驱动程序将只使用ID为2和3的设备，并且会将设备ID分别映射为0和1。

## 第3章 CUDA执行模型

### 3.1 CUDA执行模型概述

GPU架构是围绕一个`流式多处理器（SM）`的可扩展阵列搭建的。可以通过复制这种架构的构建块来实现GPU的硬件并行。Fermi SM的关键组件：

- CUDA核心
- 共享内存/一级缓存
- 寄存器文件
- 加载/存储单元
- 特殊功能单元
- 线程束调度器

<u>GPU中的每一个SM都能支持数百个线程并发执行，每个GPU通常有多个SM，所以在一个GPU上并发执行数千个线程是有可能的。当启动一个内核网格时，它的线程块被分布在了可用的SM上来执行。线程块一旦被调度到一个SM上，其中的线程只会在那个指定的SM上并发执行。多个线程块可能会被分配到同一个SM上，而且是根据SM资源的可用性进行调度的。同一线程中的指令利用指令级并行性进行流水线化。</u>

**CUDA采用单指令多线程（SIMT）架构来管理和执行线程，每32个线程为一组，被称为线程束（warp）。**线程束中的所有线程同时执行相同的指令。每个线程都有自己的指令地址计数器和寄存器状态，利用自身的数据执行当前的指令。每个SM都将分配给它的线程块划分到包含32个线程的线程束中，然后在可用的硬件资源上调度执行。

SIMT架构与SIMD（单指令多数据）架构相似。两者都是将相同的指令广播给多个执行单元来实现并行。**一个关键的区别是SIMD要求同一个向量中的所有元素要在一个统一的同步组中一起执行，而SIMT允许属于同一线程束的多个线程独立执行。尽管一个线程束中的所有线程在相同的程序地址上同时开始执行，但是单独的线程仍有可能有不同的行为。**SIMT确保可以编写独立的线程级并行代码、标量线程以及用于协调线程的数据并行代码。SIMT模型包含3个SIMD所不具备的关键特征。

- 每个线程都有自己的指令地址计数器
- 每个线程都有自己的寄存器状态
- 每个线程可以有一个独立的执行路径

>**一个神奇的数字：32**。32在CUDA程序里是一个神奇的数字。它来自于硬件系统，也对软件的性能有着重要的影响。从概念上讲，它是SM用SIMD方式所同时处理的工作粒度。优化工作负载以适应线程束（一组有32个线程）的边界，一般这样会更有效地利用GPU计算资源。
>**SM：GPU架构的核心。**SM是GPU架构的核心。寄存器和共享内存是SM中的稀缺资源。CUDA将这些资源分配到SM中的所有常驻线程里。因此，这些有限的资源限制了在SM上活跃的线程束数量，活跃的线程束数量对应于SM上的并行量。

一个线程块只能在一个SM上被调度。一旦线程块在一个SM上被调度，就会保存在该SM上直到执行完成。在同一时间，一个SM可以容纳多个线程块。在SM中，共享内存和寄存器是非常重要的资源。共享内存被分配在SM上的常驻线程块中，寄存器在线程中被分配。线程块中的线程通过这些资源可以进行相互的合作和通信。尽管线程块里的所有线程都可以逻辑地并行运行，但是并不是所有线程都可以同时在物理层面执行。因此，线程块里的不同线程可能会以不同的速度前进。

**并发竞争：**在并行线程中共享数据可能会引起竞争：多个线程使用未定义的顺序访问同一个数据，从而导致不可预测的程序行为。CUDA提供了一种用来同步线程块里的线程的方法，从而保证所有线程在进一步动作之前都达到执行过程中的一个特定点。然而，没有提供块间同步的原语。

**调度：**尽管线程块里的线程束可以任意顺序调度，但活跃的线程束的数量还是会由SM的资源所限制。当线程束由于任何理由闲置的时候（如等待从设备内存中读取数值），SM可以从同一SM上的常驻线程块中调度其他可用的线程束。在并发的线程束间切换并没有开销，因为硬件资源已经被分配到了SM上的所有线程和块中，所以最新被调度的线程束的状态已经存储在SM上。

#### 3.1.1 GPU架构概述

#### 3.1.2 Fermi架构

一个SM，包含了以下内容：

- 执行单元（CUDA核心）
- 调度线程束的调度器和调度单元
- 共享内存、寄存器文件和一级缓存

每一个多处理器有16个加载/存储单元（如图3-1所示），允许每个时钟周期内有16个线程（线程束的一半）计算源地址和目的地址。`特殊功能单元（SFU）`执行固有指令，如正弦、余弦、平方根和插值。每个SFU在每个时钟周期内的每个线程上执行一个固有指令。

#### 3.1.3 Kepler架构

Kepler架构的3个重要的创新。

- 强化的SM
- 动态并行
- Hyper-Q技术

`动态并行`是Kepler GPU的一个新特性，它允许GPU动态启动新的网格。有了这个特点，任一内核都能启动其他的内核，并且管理任何核间需要的依赖关系来正确地执行附加的工作。这一特点也让你更容易创建和优化递归及与数据相关的执行模式。有了动态并行，GPU能够启动嵌套内核，消除了与CPU通信的需求。动态并行拓宽了GPU在各种学科上的适用性。动态地启动小型和中型的并行工作负载，这在以前是需要很高代价的。

`Hyper-Q技术`增加了更多的CPU和GPU之间的同步硬件连接，以确保CPU核心能够在GPU上同时运行更多的任务。因此，当使用Kepler GPU时，既可以增加GPU的利用率，也可以减少CPU的闲置时间。

#### 3.1.4 配置文件驱动优化

性能分析是通过检测来分析程序性能的行为：

- 应用程序代码的空间（内存）或时间复杂度
- 特殊指令的使用
- 函数调用的频率和持续时间

开发一个HPC应用程序通常包括两个主要步骤：

- 提高代码的正确性
- 提高代码的性能

配置文件驱动的发展对于CUDA编程尤为重要，原因主要有以下几个方面。

- 一个单纯的内核应用一般不会产生最佳的性能。性能分析工具能帮助你找到代码中影响性能的关键部分，也就是性能瓶颈。
- CUDA将SM中的计算资源在该SM中的多个常驻线程块之间进行分配。这种分配形式导致一些资源成为了性能限制者。性能分析工具能帮助我们理解计算资源是如何被利用的。
- CUDA提供了一个硬件架构的抽象，它能够让用户控制线程并发。性能分析工具可以检测和优化，并将优化可视化。

在CUDA性能分析中，事件是可计算的活动，它对应一个在内核执行期间被收集的硬件计数器。指标是内核的特征，它由一个或多个事件计算得到。请记住以下概念事件和指标：

- 大多数计数器通过流式多处理器来报告，而不是通过整个GPU。
- 一个单一的运行只能获得几个计数器。有些计数器的获得是相互排斥的。多个性能分析运行往往需要获取所有相关的计数器。
- 由于GPU执行中的变化（如线程块和线程束调度指令），经重复运行，计数器值可能不是完全相同的。

有3种常见的限制内核性能的因素：

- 存储带宽
- 计算资源
- 指令和内存延迟

### 3.2 理解线程束执行的本质

#### 3.2.1 线程束和线程块

**线程束是SM中基本的执行单元。当一个线程块的网格被启动后，网格中的线程块分布在SM中。一旦线程块被调度到一个SM上，线程块中的线程会被进一步划分为线程束。一个线程束由32个连续的线程组成，在一个线程束中，所有的线程按照单指令多线程（SIMT）方式执行；也就是说，所有线程都执行相同的指令，每个线程在私有数据上进行操作。**例如，一个有128个线程的一维线程块被组织到4个线程束里。

对于一个给定的二维线程块，在一个块中每个线程的独特标识符都可以用内置变量`threadIdx`和`blockDim`来计算：

```c
threadIdx.y * blockDim.x + threadIdx.x
```

对于一个三维线程块，计算如下：

```c
threadIdx.z * blockDim.y * blockDim.x + threadIdx.y * blockDim.x + threadIdx.x
```

一个线程块的线程束的数量可以根据下式确定：

$$
N=向正无穷取整(\frac{线程块中线程数量}{WarpSize})
$$

硬件总是给一个线程块分配一定数量的线程束。线程束不会在不同的线程块之间分离。如果线程块的大小不是线程束大小的偶数倍，那么在最后的线程束里有些线程就不会活跃。

>从逻辑角度来看，线程块是线程的集合，它们可以被组织为一维、二维或三维布局。从硬件角度来看，线程块是一维线程束的集合。在线程块中线程被组织成一维布局，每32个连续线程组成一个线程束。

#### 3.2.2 线程束分化

GPU是相对简单的设备，它没有复杂的分支预测机制。一个线程束中的所有线程在同一周期中必须执行相同的指令，如果一个线程执行一条指令，那么线程束中的所有线程都必须执行该指令。如果在同一线程束中的线程使用不同的路径通过同一个应用程序，这可能会产生问题。

```c
if (cond) {
    ...
} else {
    ...
}
```

假设在一个线程束中有16个线程执行这段代码，cond为true，但对于其他16个来说cond为false。一半的线程束需要执行if语句块中的指令，而另一半需要执行else语句块中的指令。**在同一线程束中的线程执行不同的指令，被称为线程束分化。**如果一个线程束中的线程产生分化，线程束将连续执行每一个分支路径，而禁用不执行这一路径的线程。线程束分化会导致性能明显地下降。在前面的例子中可以看到，线程束中并行线程的数量减少了一半：只有16个线程同时活跃地执行，而其他16个被禁用了。条件分支越多，并行性削弱越严重。**注意，线程束分化只发生在同一个线程束中。在不同的线程束中，不同的条件值不会引起线程束分化。**

为了获得最佳的性能，应该避免在同一线程束中有不同的执行路径。请记住，在一个线程块中，线程的线程束分配是确定的。因此，以这样的方式对数据进行分区是可行的（尽管不是微不足道的，但取决于算法），以确保同一个线程束中的所有线程在一个应用程序中使用同一个控制路径。例如，假设有两个分支，下面展示了简单的算术内核示例`mathKernel1`。我们可以用一个偶数和奇数线程方法来模拟一个简单的数据分区，目的是导致线程束分化。该条件（tid%2==0）使偶数编号的线程执行if子句，奇数编号的线程执行else子句。如果使用线程束方法（而不是线程方法）来交叉存取数据，可以避免线程束分化，并且设备的利用率可达到100%。`mathKernel2`条件`（tid/warpSize）%2==0`使分支粒度是线程束大小的倍数；偶数编号的线程执行if子句，奇数编号的线程执行else子句。这个核函数产生相同的输出，但是顺序不同。

[simpleDivergence.cu](https://github.com/Jianliang-Shen/cuda-c-samples/blob/main/chapter03/simpleDivergence.cu)简单的线程束分化。注：代码中iStart等需改为double。

因为在设备上第一次运行可能会增加间接开销，并且在此处测量的性能是非常精细的，所以，添加了一个额外的内核启动（warmingup，与mathKernel2一样）来去除这一间接开销。

```c
__global__ void mathKernel1(float *c)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    float ia, ib;
    ia = ib = 0.0f;

    if (tid % 2 == 0)
    {
        ia = 100.0f;
    }
    else
    {
        ib = 200.0f;
    }

    c[tid] = ia + ib;
}

__global__ void mathKernel2(float *c)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    float ia, ib;
    ia = ib = 0.0f;

    if ((tid / warpSize) % 2 == 0)
    {
        ia = 100.0f;
    }
    else
    {
        ib = 200.0f;
    }

    c[tid] = ia + ib;
}

__global__ void mathKernel3(float *c)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    float ia, ib;
    ia = ib = 0.0f;

    bool ipred = (tid % 2 == 0);

    if (ipred)
    {
        ia = 100.0f;
    }

    if (!ipred)
    {
        ib = 200.0f;
    }

    c[tid] = ia + ib;
}

__global__ void mathKernel4(float *c)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    float ia, ib;
    ia = ib = 0.0f;

    int itid = tid >> 5;

    if (itid & 0x01 == 0)
    {
        ia = 100.0f;
    }
    else
    {
        ib = 200.0f;
    }

    c[tid] = ia + ib;
}

__global__ void warmingup(float *c)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    float ia, ib;
    ia = ib = 0.0f;

    if ((tid / warpSize) % 2 == 0)
    {
        ia = 100.0f;
    }
    else
    {
        ib = 200.0f;
    }

    c[tid] = ia + ib;
}
```

```txt
./simpleDivergence 32 64                    
./simpleDivergence using Device 0: Orin
Data size 64 Execution Configure (block 32 grid 2)
warmup      <<<    2   32 >>> elapsed 0.000557 sec 
mathKernel1 <<<    2   32 >>> elapsed 0.000036 sec 
mathKernel2 <<<    2   32 >>> elapsed 0.000037 sec 
mathKernel3 <<<    2   32 >>> elapsed 0.000029 sec 
mathKernel4 <<<    2   32 >>> elapsed 0.000026 sec
```

分支效率被定义为未分化的分支与全部分支之比，可以使用以下公式来计算：

$$
分支效率 = \frac{分支数 - 分支分化数}{分支数} * 100
$$

奇怪的是，没有报告显示出有分支分化（即分支效率是100%）。这个奇怪的现象是CUDA编译器优化导致的结果，它将短的、有条件的代码段的断定指令取代了分支指令（导致分化的实际控制流指令）。在分支预测中，根据条件，把每个线程中的一个断定变量设置为1或0。这两种条件流路径被完全执行，但只有断定为1的指令被执行。断定为0的指令不被执行，但相应的线程也不会停止。这和实际的分支指令之间的区别是微妙的，但理解它很重要。只有在条件语句的指令数小于某个阈值时，编译器才用断定指令替换分支指令。**因此，一段很长的代码路径肯定会导致线程束分化。**

```c
__global__ void mathKernel1(float *c)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    float ia, ib;
    ia = ib = 0.0f;

    bool ipred = (tid % 2 == 0);

    if (ipred)
    {
        ia = 100.0f;
    }
    
    if (!ipred)
    {
        ib = 200.0f;
    }

    c[tid] = ia + ib;
}
```

```txt
nvcc -g -G simpleDivergence.cu -o simpleDivergence # 可以强制CUDA编译器不利用分支预测去优化内核
./simpleDivergence 32 64
./simpleDivergence using Device 0: Orin
Data size 64 Execution Configure (block 32 grid 2)
warmup      <<<    2   32 >>> elapsed 0.000550 sec 
mathKernel1 <<<    2   32 >>> elapsed 0.000054 sec 
mathKernel2 <<<    2   32 >>> elapsed 0.000039 sec 
mathKernel3 <<<    2   32 >>> elapsed 0.000033 sec 
mathKernel4 <<<    2   32 >>> elapsed 0.000028 sec
```

- 当一个分化的线程采取不同的代码路径时，会产生线程束分化
- 不同的if-then-else分支会连续执行
- 尝试调整分支粒度以适应线程束大小的倍数，避免线程束分化
- 不同的分化可以执行不同的代码且无须以牺牲性能为代价

#### 3.2.3 资源分配

线程束的本地执行上下文主要由以下资源组成：

- 程序计数器
- 寄存器
- 共享内存

由SM处理的每个线程束的执行上下文，在整个线程束的生存期中是保存在芯片内的。因此，从一个执行上下文切换到另一个执行上下文没有损失。每个SM都有32位的寄存器组，它存储在寄存器文件中，并且可以在线程中进行分配，同时固定数量的共享内存用来在线程块中进行分配。**对于一个给定的内核，同时存在于同一个SM中的线程块和线程束的数量取决于在SM中可用的且内核所需的寄存器和共享内存的数量。**

- 若每个线程消耗的寄存器越多，则可以放在一个SM中的线程束就越少。如果可以减少内核消耗寄存器的数量，那么就可以同时处理更多的线程束。
- 若一个线程块消耗的共享内存越多，则在一个SM中可以被同时处理的线程块就会变少。如果每个线程块使用的共享内存数量变少，那么可以同时处理更多的线程块。

当计算资源（如寄存器和共享内存）已分配给线程块时，线程块被称为活跃的块。它所包含的线程束被称为活跃的线程束。活跃的线程束可以进一步被分为以下3种类型：

- 选定的线程束
- 阻塞的线程束
- 符合条件的线程束

一个SM上的线程束调度器在每个周期都选择活跃的线程束，然后把它们调度到执行单元。活跃执行的线程束被称为选定的线程束。如果一个活跃的线程束准备执行但尚未执行，它是一个符合条件的线程束。如果一个线程束没有做好执行的准备，它是一个阻塞的线程束。如果同时满足以下两个条件则线程束符合执行条件。

- 32个CUDA核心可用于执行
- 当前指令中所有的参数都已就绪

#### 3.2.4 延迟隐藏

SM依赖线程级并行，以最大化功能单元的利用率，因此，利用率与常驻线程束的数量直接相关。**在指令发出和完成之间的时钟周期被定义为指令延迟。当每个时钟周期中所有的线程调度器都有一个符合条件的线程束时，可以达到计算资源的完全利用。这就可以保证，通过在其他常驻线程束中发布其他指令，可以隐藏每个指令的延迟。**与在CPU上用C语言编程相比，延迟隐藏在CUDA编程中尤为重要。CPU核心是为同时最小化延迟一个或两个线程而设计的，而GPU则是为处理大量并发和轻量级线程以最大化吞吐量而设计的。GPU的指令延迟被其他线程束的计算隐藏。考虑到指令延迟，指令可以被分为两种基本类型：

- 算术指令，算术指令延迟是一个算术操作从开始到它产生输出之间的时间。算术操作为10～20个周期
- 内存指令，内存指令延迟是指发送出的加载或存储操作和数据到达目的地之间的时间。全局内存访问为400～800个周期

如何估算隐藏延迟所需要的活跃线程束的数量。利特尔法则（Little’s Law）可以提供一个合理的近似值。它起源于队列理论中的一个定理，它也可以应用于GPU中：$$所需线程束数量 ＝ 延迟 * 吞吐量$$

假设在内核里一条指令的平均延迟是5个周期。为了保持在每个周期内执行6个线程束的吞吐量，则至少需要30个未完成的线程束。

>带宽和吞吐量经常被混淆，根据实际情况它们可以被交换使用。吞吐量和带宽都是用来度量性能的速度指标。带宽通常是指理论峰值，而吞吐量是指已达到的值。带宽通常是用来描述单位时间内最大可能的数据传输量，而吞吐量是用来描述单位时间内任何形式的信息或操作的执行速度，例如，每个周期完成多少个指令。对于算术运算来说，其所需的并行可以表示成隐藏算术延迟所需要的操作数量。算术运算是一个32位的浮点数乘加运算（a＋b×c），表示在每个SM中每个时钟周期内的操作数量。吞吐量因不同的算术指令而不同。算术运算所需的并行可以用操作的数量或线程束的数量来表示。这个简单的单位转换表明，

有两种方法可以提高并行：

- 指令级并行（ILP）：一个线程中有很多独立的指令
- 线程级并行（TLP）：很多并发地符合条件的线程

与指令延迟很像，通过在每个线程/线程束中创建更多独立的内存操作，或创建更多并发地活跃的线程/线程束，可以增加可用的并行。

延迟隐藏取决于每个SM中活跃线程束的数量，这一数量由执行配置和资源约束隐式决定（一个内核中寄存器和共享内存的使用情况）。选择一个最优执行配置的关键是在延迟隐藏和资源利用之间找到一种平衡。

>因为GPU在线程间分配计算资源并在并发线程束之间切换的消耗（在一个或两个周期命令上）很小，所以所需的状态可以在芯片内获得。如果有足够的并发活跃线程，那么可以让GPU在每个周期内的每一个流水线阶段中忙碌。在这种情况下，一个线程束的延迟可以被其他线程束的执行隐藏。因此，向SM显示足够的并行对性能是有利的。计算所需并行的一个简单的公式是，用每个SM核心的数量乘以在该SM上一条算术指令的延迟。例如，Fermi有32个单精度浮点流水线线路，一个算术指令的延迟是20个周期，所以，每个SM至少需要有32×20＝640个线程使设备处于忙碌状态。然而，这只是一个下边界。

#### 3.2.5 占用率

在每个CUDA核心里指令是顺序执行的。当一个线程束阻塞时，SM切换执行其他符合条件的线程束。理想情况下，我们想要有足够的线程束占用设备的核心。占用率是每个SM中活跃的线程束占最大线程束数量的比值。

$$
占用率 = \frac{活跃线程束数量}{最大线程束数量} * 100
$$

[simpleDeviceQuery.cu](https://github.com/Jianliang-Shen/cuda-c-samples/blob/main/chapter03/simpleDeviceQuery.cu)展示了如何使用cudaGetDeviceProperties获得GPU的配置信息。

```txt
Device 0: Orin
  Number of multiprocessors:                     8
  Total amount of constant memory:               64.00 KB
  Total amount of shared memory per block:       48.00 KB
  Total number of registers available per block: 65536
  Warp size:                                     32
  Maximum number of threads per block:           1024
  Maximum number of threads per multiprocessor:  1536
  Maximum number of warps per multiprocessor:    48
```

在确定GPU的计算能力后，物理限制部分的数据是自动填充的。接下来，需要输入以下内核资源信息：

- 每个块的线程（执行配置）
- 每个线程的寄存器（资源使用情况）
- 每个块的共享内存（资源使用情况）

每个线程的寄存器和每个块的共享内存资源的使用情况可以从nvcc中用以下编译器标志获得：

```txt
--ptxas-options=-v

nvcc simpleDivergence.cu -o simpleDivergence --ptxas-options=-v
ptxas info    : 0 bytes gmem
ptxas info    : Compiling entry function '_Z9warmingupPf' for 'sm_52'
ptxas info    : Function properties for _Z9warmingupPf
    0 bytes stack frame, 0 bytes spill stores, 0 bytes spill loads
ptxas info    : Used 10 registers, 328 bytes cmem[0], 4 bytes cmem[2]
ptxas info    : Compiling entry function '_Z11mathKernel4Pf' for 'sm_52'
ptxas info    : Function properties for _Z11mathKernel4Pf
    0 bytes stack frame, 0 bytes spill stores, 0 bytes spill loads
ptxas info    : Used 4 registers, 328 bytes cmem[0]
ptxas info    : Compiling entry function '_Z11mathKernel3Pf' for 'sm_52'
ptxas info    : Function properties for _Z11mathKernel3Pf
    0 bytes stack frame, 0 bytes spill stores, 0 bytes spill loads
ptxas info    : Used 4 registers, 328 bytes cmem[0], 4 bytes cmem[2]
ptxas info    : Compiling entry function '_Z11mathKernel2Pf' for 'sm_52'
ptxas info    : Function properties for _Z11mathKernel2Pf
    0 bytes stack frame, 0 bytes spill stores, 0 bytes spill loads
ptxas info    : Used 10 registers, 328 bytes cmem[0], 4 bytes cmem[2]
ptxas info    : Compiling entry function '_Z11mathKernel1Pf' for 'sm_52'
ptxas info    : Function properties for _Z11mathKernel1Pf
    0 bytes stack frame, 0 bytes spill stores, 0 bytes spill loads
ptxas info    : Used 4 registers, 328 bytes cmem[0], 4 bytes cmem[2]
```

内核使用的寄存器数量会对常驻线程束数量产生显著的影响。寄存器的使用可以用下面的nvcc标志手动控制：

```txt
-maxrregcount=NUM
```

`-maxrregcount`选项告诉编译器每个线程使用的寄存器数量不能超过NUM个。使用这个编译器标志，可以得到占用率计算器推荐的寄存器数量，同时使用这个数值可以改善应用程序的性能。

为了提高占用率，还需要调整线程块配置或重新调整资源的使用情况，以允许更多的线程束同时处于活跃状态和提高计算资源的利用率。极端地操纵线程块会限制资源的利用

- 小线程块：每个块中线程太少，会在所有资源被充分利用之前导致硬件达到每个SM的线程束数量的限制。
- 大线程块：每个块中有太多的线程，会导致在每个SM中每个线程可用的硬件资源较少。

>**网格和线程块大小的准则**:
>
>- 保持每个块中线程数量是线程束大小（32）的倍数
>- 避免块太小：每个块至少要有128或256个线程
>- 根据内核资源的需求调整块大小块的数量要远远多于SM的数量，从而在设备中可以显示有足够的并行
>- 通过实验得到最佳执行配置和资源使用情况

尽管在每种情况下会遇到不同的硬件限制，但它们都会导致计算资源未被充分利用，阻碍隐藏指令和内存延迟的并行的建立。占用率唯一注重的是在每个SM中并发线程或线程束的数量。然而，充分的占用率不是性能优化的唯一目标。内核一旦达到一定级别的占用率，进一步增加占用率可能不会改进性能。

#### 3.2.6 同步

栅栏同步是一个原语，它在许多并行编程语言中都很常见。在CUDA中，同步可以在两个级别执行：

- 系统级：等待主机和设备完成所有的工作
- 块级：在设备执行过程中等待一个线程块中所有线程到达同一点

对于主机来说，由于许多CUDA API调用和所有的内核启动不是同步的，`cudaDeviceSynchronize`函数可以用来阻塞主机应用程序，直到所有的CUDA操作（复制、 核函数等）完成。

因为在一个线程块中线程束以一个未定义的顺序被执行，CUDA提供了一个使用块局部栅栏来同步它们的执行的功能。使用下述函数在内核中标记同步点：

```c
__device__ void __syncthreads(void);
```

当`__syncthreads`被调用时，在同一个线程块中每个线程都必须等待直至该线程块中所有其他线程都已经达到这个同步点。在栅栏之前所有线程产生的所有全局内存和共享内存访问，将会在栅栏后对线程块中所有其他的线程可见。该函数可以协调同一个块中线程之间的通信，但它强制线程束空闲，从而可能对性能产生负面影响。

线程块中的线程可以通过共享内存和寄存器来共享数据。当线程之间共享数据时，要避免竞争条件。竞争条件或危险，是指多个线程无序地访问相同的内存位置。例如，当一个位置的无序读发生在写操作之后时，写后读竞争条件发生。因为读和写之间没有顺序，所以读应该在写前还是在写后加载值是未定义的。其他竞争条件的例子有读后写或写后写。当线程块中的线程在逻辑上并行运行时，在物理上并不是所有的线程都可以在同一时间执行。如果线程A试图读取由线程B在不同的线程束中写的数据，若使用了适当的同步，只需确定线程B已经写完就可以了。否则，会出现竞争条件。

在不同的块之间没有线程同步。块间同步，唯一安全的方法是在每个内核执行结束端使用全局同步点；也就是说，在全局同步之后，终止当前的核函数，开始执行新的核函数。不同块中的线程不允许相互同步，因此GPU可以以任意顺序执行块。这使得CUDA程序在大规模并行GPU上是可扩展的。

#### 3.2.7 可扩展性

对于任何并行应用程序而言，可扩展性是一个理想的特性。可扩展性意味着为并行应用程序提供了额外的硬件资源，相对于增加的资源，并行应用程序会产生加速。例如，若一个CUDA程序在两个SM中是可扩展的，则与在一个SM中运行相比，在两个SM中运行会使运行时间减半。一个可扩展的并行程序可以高效地使用所有的计算资源以提高性能。可扩展性意味着增加的计算核心可以提高性能。串行代码本身是不可扩展的，因为在成千上万的内核上运行一个串行单线程应用程序，对性能是没有影响的。并行代码有可扩展的潜能，但真正的可扩展性取决于算法设计和硬件特性。

**能够在可变数量的计算核心上执行相同的应用程序代码的能力被称为透明可扩展性。**一个透明的可扩展平台拓宽了现有应用程序的应用范围，并减少了开发人员的负担，因为它们可以避免新的或不同的硬件产生的变化。可扩展性比效率更重要。一个可扩展但效率很低的系统可以通过简单添加硬件核心来处理更大的工作负载。一个效率很高但不可扩展的系统可能很快会达到可实现性能的上限。

CUDA内核启动时，线程块分布在多个SM中。网格中的线程块以并行或连续或任意的顺序被执行。这种独立性使得CUDA程序在任意数量的计算核心间可以扩展。若GPU有两个SM，可以同时执行两个块；假使GPU有4个SM，则可以同时执行4个块。不修改任何代码，一个应用程序可以在不同的GPU配置上运行，并且所需的执行时间根据可用的资源而改变。

### 3.3 并行性的表现

[sumMatrix.cu](https://github.com/Jianliang-Shen/cuda-c-samples/blob/main/chapter03/sumMatrix.cu)对块和网格配置执行试验。

#### 3.3.1 用nvprof检测活跃的线程束

```txt
➜  chapter03 ./sumMatrix 32 32             
sumMatrixOnGPU2D <<<(128,128), (32,32)>>> elapsed 0.005473 ms

➜  chapter03 ./sumMatrix 32 16             
sumMatrixOnGPU2D <<<(128,256), (32,16)>>> elapsed 0.003788 ms

➜  chapter03 ./sumMatrix 16 32             
sumMatrixOnGPU2D <<<(256,128), (16,32)>>> elapsed 0.003902 ms

➜  chapter03 ./sumMatrix 16 16             
sumMatrixOnGPU2D <<<(256,256), (16,16)>>> elapsed 0.003748 ms
```

一个内核的可实现占用率被定义为：每周期内活跃线程束的平均数量与一个SM支持的线程束最大数量的比值。

因为第二种情况中的块数比第一种情况的多，所以设备就可以有更多活跃的线程束。其原因可能是第二种情况与第一种情况相比有更高的可实现占用率和更好的性能。第四种情况有最高的可实现占用率，但它不是最快的，因此，更高的占用率并不一定意味着有更高的性能。肯定有其他因素限制GPU的性能。（这里和orin上测试结果不一致）

#### 3.3.2 用nvprof检测内存操作

在`sumMatrix`内核（`C[idx]＝A[idx]＋B[idx]`）中有3个内存操作：两个内存加载和一个内存存储。

第四种情况中的加载吞吐量最高，第二种情况中的加载吞吐量大约是第四种情况的一半，但第四种情况却比第二种情况慢。所以，更高的加载吞吐量并不一定意味着更高的性能。最后两种情况下的加载效率是最前面两种情况的一半。这可以解释为什么最后两种情况下更高的加载吞吐量和可实现占用率没有产生较好的性能。尽管在最后两种情况下正在执行的加载数量（即吞吐量）很多，但是那些加载的有效性（即效率）是较低的。

最后两种情况的共同特征是它们在最内层维数中块的大小是线程束的一半。如前所述，对网格和块启发式算法来说，最内层的维数应该总是线程束大小的倍数。

#### 3.3.3 增大并行性

**从前一节可以总结出，一个块的最内层维数（block.x）应该是线程束大小的倍数。**这样能极大地提高了加载效率。

```txt
➜  chapter03 ./sumMatrix 64 2
sumMatrixOnGPU2D <<<(64,2048), (64,2)>>> elapsed 0.003877 ms
➜  chapter03 ./sumMatrix 64 4
sumMatrixOnGPU2D <<<(64,1024), (64,4)>>> elapsed 0.003864 ms
➜  chapter03 ./sumMatrix 64 8
sumMatrixOnGPU2D <<<(64,512), (64,8)>>> elapsed 0.003923 ms
➜  chapter03 ./sumMatrix 128 2
sumMatrixOnGPU2D <<<(32,2048), (128,2)>>> elapsed 0.003818 ms
➜  chapter03 ./sumMatrix 128 4
sumMatrixOnGPU2D <<<(32,1024), (128,4)>>> elapsed 0.003972 ms
➜  chapter03 ./sumMatrix 128 8
sumMatrixOnGPU2D <<<(32,512), (128,8)>>> elapsed 0.005476 ms
➜  chapter03 ./sumMatrix 256 2
sumMatrixOnGPU2D <<<(16,2048), (256,2)>>> elapsed 0.003937 ms
➜  chapter03 ./sumMatrix 256 4
sumMatrixOnGPU2D <<<(16,1024), (256,4)>>> elapsed 0.006425 ms
➜  chapter03 ./sumMatrix 256 8
sumMatrixOnGPU2D <<<(16,512), (256,8)>>> elapsed 0.000010 ms
Error: sumMatrix.cu:140, code: 9, reason: invalid configuration argument
```

- 最后一次的执行配置块的大小为（256，8），这是无效的。一个块中线程总数超过了1024个（这是GPU的硬件限制）。
- 最好的结果是第四种情况，块大小为（128，2）。
- 第一种情况中块大小为（64，2），尽管在这种情况下启动的线程块最多，但不是最快的配置。
- 因为第二种情况中块的配置为（64，4），与最好的情况有相同数量的线程块，这两种情况应该在设备上显示出相同的并行性。因为这种情况相比（128，2）仍然表现较差，所以你可以得出这样的结论：线程块最内层维度的大小对性能起着的关键的作用。这正重复了前一节中总结的结论。
- 在所有其他情况下，线程块的数量都比最好的情况少。因此，增大并行性仍然是性能优化的一个重要因素。

>指标与性能
>
>- 在大部分情况下，一个单独的指标不能产生最佳的性能
>- 与总体性能最直接相关的指标或事件取决于内核代码的本质
>- 在相关的指标与事件之间寻求一个好的平衡
>- 从不同角度查看内核以寻找相关指标间的平衡
>- 网格/块启发式算法为性能调节提供了一个很好的起点

### 3.4 避免分支分化

有时，控制流依赖于线程索引。线程束中的条件执行可能引起线程束分化，这会导致内核性能变差。通过重新组织数据的获取模式，可以减少或避免线程束分化。

#### 3.4.1 并行归约问题

假设要对一个有N个元素的整数数组求和，如何通过并行计算快速求和呢？鉴于加法的结合律和交换律，数组元素可以以任何顺序求和。所以可以用以下的方法执行并行加法运算：

- 将输入向量划分到更小的数据块中。
- 用一个线程计算一个数据块的部分和。
- 对每个数据块的部分和再求和得出最终结果。

根据每次迭代后输出元素就地存储的位置，成对的并行求和实现可以被进一步分为以下两种类型：

- 相邻配对：元素与它们直接相邻的元素配对
- 交错配对：根据给定的跨度配对元素

在每一步实现中，一个线程对两个相邻元素进行操作，产生部分和。对于有N个元素的数组，这种实现方式需要N―1次求和，进行`log2N`步。
值得注意的是，在这种实现方法的每一步中，一个线程的输入是输入数组长度的一半。

尽管以上实现的是加法，但任何满足交换律和结合律的运算都可以代替加法。例如，通过调用max代替求和运算，就可以计算输入向量中的最大值。其他有效运算的例子有最小值、平均值和乘积。**在向量中执行满足交换律和结合律的运算，被称为归约问题。并行归约问题是这种运算的并行执行。并行归约是一种最常见的并行模式，并且是许多并行算法中的一个关键运算。**

#### 3.4.2 并行归约中的分化

在这个内核里，有两个全局内存数组：一个大数组用来存放整个数组，进行归约；另一个小数组用来存放每个线程块的部分和。每个线程块在数组的一部分上独立地执行操作。循环中迭代一次执行一个归约步骤。归约是在就地完成的，这意味着在每一步，全局内存里的值都被部分和替代。__syncthreads语句可以保证，线程块中的任一线程在进入下一次迭代之前，在当前迭代里每个线程的所有部分和都被保存在了全局内存中。进入下一次迭代的所有线程都使用上一步产生的数值。在最后一个循环以后，整个线程块的和被保存进全局内存中。

```c
// Neighbored Pair Implementation with divergence
__global__ void reduceNeighbored (int *g_idata, int *g_odata, unsigned int n)
{
    // set thread ID
    unsigned int tid = threadIdx.x;
    unsigned int idx = blockIdx.x * blockDim.x + threadIdx.x;

    // convert global data pointer to the local pointer of this block
    int *idata = g_idata + blockIdx.x * blockDim.x;

    // boundary check
    if (idx >= n) return;

    // in-place reduction in global memory
    for (int stride = 1; stride < blockDim.x; stride *= 2)
    {
        if ((tid % (2 * stride)) == 0)
        {
            idata[tid] += idata[tid + stride];
        }

        // synchronize within threadblock
        __syncthreads();
    }

    // write result for this block to global mem
    if (tid == 0) g_odata[blockIdx.x] = idata[0];
}

    // main

    // initialization
    int size = 1 << 24; // total number of elements to reduce
    printf("    with array size %d  ", size);

    // execution configuration
    int blocksize = 512;   // initial block size

    if(argc > 1)
    {
        blocksize = atoi(argv[1]);   // block size from command line argument
    }

    dim3 block (blocksize, 1);
    dim3 grid  ((size + block.x - 1) / block.x, 1);
    printf("grid %d block %d\n", grid.x, block.x);

    // allocate host memory
    size_t bytes = size * sizeof(int);
    int *h_idata = (int *) malloc(bytes);
    int *h_odata = (int *) malloc(grid.x * sizeof(int));
    int *tmp     = (int *) malloc(bytes);

    // initialize the array
    for (int i = 0; i < size; i++)
    {
        // mask off high 2 bytes to force max number to 255
        h_idata[i] = (int)( rand() & 0xFF );
    }

    memcpy (tmp, h_idata, bytes);

    double iStart, iElaps;
    int gpu_sum = 0;

    // allocate device memory
    int *d_idata = NULL;
    int *d_odata = NULL;
    CHECK(cudaMalloc((void **) &d_idata, bytes));
    CHECK(cudaMalloc((void **) &d_odata, grid.x * sizeof(int)));

    // kernel 1: reduceNeighbored
    CHECK(cudaMemcpy(d_idata, h_idata, bytes, cudaMemcpyHostToDevice));
    CHECK(cudaDeviceSynchronize());
    iStart = seconds();
    reduceNeighbored<<<grid, block>>>(d_idata, d_odata, size);
    CHECK(cudaDeviceSynchronize());
    iElaps = seconds() - iStart;
    CHECK(cudaMemcpy(h_odata, d_odata, grid.x * sizeof(int),
                     cudaMemcpyDeviceToHost));
    gpu_sum = 0;

    for (int i = 0; i < grid.x; i++) gpu_sum += h_odata[i];

    printf("gpu Neighbored  elapsed %f sec gpu_sum: %d <<<grid %d block "
           "%d>>>\n", iElaps, gpu_sum, grid.x, block.x);
```

![归约](/img/post_pics/cuda-c/1.jpeg)

**两个相邻元素间的距离被称为跨度，初始化均为1**。在每一次归约循环结束后，这个间隔就被乘以2。在第一次循环结束后，idata（全局数据指针）的偶数元素将会被部分和替代。在第二次循环结束后，idata的每四个元素将会被新产生的部分和替代。因为线程块间无法同步，所以每个线程块产生的部分和被复制回了主机，并且在那儿进行串行求和。

[reduceInteger.cu](https://github.com/Jianliang-Shen/cuda-c-samples/blob/main/chapter03/reduceInteger.cu)进行整数归约。

![归约](/img/post_pics/cuda-c/2.jpeg)

```txt
./reduceInteger starting reduction at device 0: Orin     with array size 16777216  grid 32768 block 512
cpu reduce      elapsed 0.081278 sec cpu_sum: 2139353471
gpu Neighbored  elapsed 0.012773 sec gpu_sum: 2139353471 <<<grid 32768 block 512>>>  # 基准数据
gpu Neighbored2 elapsed 0.006998 sec gpu_sum: 2139353471 <<<grid 32768 block 512>>>
gpu Interleaved elapsed 0.006103 sec gpu_sum: 2139353471 <<<grid 32768 block 512>>>  # 交错归约
gpu Unrolling2  elapsed 0.003359 sec gpu_sum: 2139353471 <<<grid 16384 block 512>>>
gpu Unrolling4  elapsed 0.001816 sec gpu_sum: 2139353471 <<<grid 8192 block 512>>>
gpu Unrolling8  elapsed 0.001327 sec gpu_sum: 2139353471 <<<grid 4096 block 512>>>
gpu UnrollWarp8 elapsed 0.001362 sec gpu_sum: 2139353471 <<<grid 4096 block 512>>>
gpu Cmptnroll8  elapsed 0.001365 sec gpu_sum: 2139353471 <<<grid 4096 block 512>>>
gpu Cmptnroll   elapsed 0.001360 sec gpu_sum: 2139353471 <<<grid 4096 block 512>>>
```

#### 3.4.3 改善并行归约的分化

```c
if ((tid % (2 * stride)) == 0)
```

因为上述语句只对偶数ID的线程为true，所以这会导致很高的线程束分化。在并行归约的第一次迭代中，只有ID为偶数的线程执行这个条件语句的主体，但是所有的线程都必须被调度。在第二次迭代中，只有四分之一的线程是活跃的，但是所有的线程仍然都必须被调度。通过重新组织每个线程的数组索引来强制ID相邻的线程执行求和操作，线程束分化就能被归约了。和之前实现相比，部分和的存储位置并没有改变，但是工作线程已经更新了。**注意看线程id不再是间隔的。**

![](/img/post_pics/cuda-c/3.jpeg)

```c
for (int stride = 1; stride < blockDim.x; stride *= 2)
    {
        // convert tid into local array index
        int index = 2 * stride * tid;

        if (index < blockDim.x)
        {
            idata[index] += idata[index + stride];
        }

        // synchronize within threadblock
        __syncthreads();
    }
```

因为跨度乘以了2，所以下面的语句使用线程块的前半部分来执行求和操作。

对于一个有512个线程（16个线程束）的块来说，前8个线程束执行第一轮归约，剩下8个线程束什么也不做。在第二轮里，前4个线程束执行归约，剩下12个线程束什么也不做。因此，这样就彻底不存在分化了。在最后五轮中，当每一轮的线程总数小于线程束的大小时，分化就会出现。在下一节将会介绍如何处理这一问题。

#### 3.4.4 交错配对的归约

与相邻配对方法相比，交错配对方法颠倒了元素的跨度。初始跨度是线程块大小的一半，然后在每次迭代中减少一半。在每次循环中，每个线程对两个被当前跨度隔开的元素进行求和，以产生一个部分和。与上面改善并行归约案例相比，交错归约的工作线程没有变化。但是，每个线程在全局内存中的加载/存储位置是不同的。

![](/img/post_pics/cuda-c/4.jpeg)

```c
// in-place reduction in global memory
    for (int stride = blockDim.x / 2; stride > 0; stride >>= 1)
    {
        if (tid < stride)
        {
            idata[tid] += idata[tid + stride];
        }

        __syncthreads();
    }
```

### 3.5 展开循环

**循环展开是一个尝试通过减少分支出现的频率和循环维护指令来优化循环的技术。**在循环展开中，循环主体在代码中要多次被编写，而不是只编写一次循环主体再使用另一个循环来反复执行的。任何的封闭循环可将它的迭代次数减少或完全删除。**循环体的复制数量被称为循环展开因子，迭代次数就变为了原始循环迭代次数除以循环展开因子。**在顺序数组中，当循环的迭代次数在循环执行之前就已经知道时，循环展开是最有效提升性能的方法。例如：

```c
for (int i = 0; i < 100; i ++){
    a[i] = b[i] + c[i];
}

for (int i = 0; i < 100; i += 2){
    a[i] = b[i] + c[i];
    a[i+1] = b[i+1] + c[i+1];
}
```

从高级语言层面上来看，循环展开使性能提高的原因可能不是显而易见的。这种提升来自于编译器执行循环展开时低级指令的改进和优化。例如，在前面循环展开的例子中，条件i<100只检查了50次，而在原来的循环中则检查了100次。另外，因为在每个循环中每个语句的读和写都是独立的，所以CPU可以同时发出内存操作。在CUDA中，循环展开的意义非常重大。我们的目标仍然是相同的：通**过减少指令消耗和增加更多的独立调度指令来提高性能。因此，更多的并发操作被添加到流水线上，以产生更高的指令和内存带宽。这为线程束调度器提供更多符合条件的线程束，它们可以帮助隐藏指令或内存延迟。**

#### 3.5.1 展开的归约

你可能会注意到，在`reduceInterleaved`核函数中每个线程块只处理一部分数据，这些数据可以被认为是一个数据块。如果用一个线程块手动展开两个数据块的处理，会怎么样？以下的核函数是`reduceInterleaved`核函数的修正版：每个线程块汇总了来自两个数据块的数据。这是一个循环分区（在第1章中已介绍）的例子，每个线程作用于多个数据块，并处理每个数据块的一个元素：

```c
__global__ void reduceUnrolling2 (int *g_idata, int *g_odata, unsigned int n)
{
    // set thread ID
    unsigned int tid = threadIdx.x;
    unsigned int idx = blockIdx.x * blockDim.x * 2 + threadIdx.x;

    // convert global data pointer to the local pointer of this block
    int *idata = g_idata + blockIdx.x * blockDim.x * 2;

    // unrolling 2
    if (idx + blockDim.x < n) g_idata[idx] += g_idata[idx + blockDim.x];

    __syncthreads();

    // in-place reduction in global memory
    for (int stride = blockDim.x / 2; stride > 0; stride >>= 1)
    {
        if (tid < stride)
        {
            idata[tid] += idata[tid + stride];
        }

        // synchronize within threadblock
        __syncthreads();
    }

    // write result for this block to global mem
    if (tid == 0) g_odata[blockIdx.x] = idata[0];
}
```

注意要在核函数的开头添加的下述语句。在这里，每个线程都添加一个来自于相邻数据块的元素。从概念上来讲，可以把它作为归约循环的一个迭代，此循环可在数据块间归约：

```c
if (idx + blockDim.x < n) g_idata[idx] += g_idata[idx + blockDim.x];
```

因为现在每个线程块处理两个数据块，我们需要调整内核的执行配置，将网格大小减小至一半：

```c
reduceUnrolling2<<<grid.x / 2, block>>>(d_idata, d_odata, size);
```

即使只进行简单的更改，现在核函数的执行速度比原来快3.80倍。展开4次，提升7.03倍，展开8次，提升9.6倍。正如预想的一样，在一个线程中有更多的独立内存加载/存储操作会产生更好的性能，因为内存延迟可以更好地被隐藏起来。可以使用设备内存读取吞吐量指标，以确定这就是性能提高的原因。

#### 3.5.2 展开线程的归约

`__syncthreads`是用于块内同步的。在归约核函数中，它用来确保在线程进入下一轮之前，每一轮中所有线程已经将局部结果写入全局内存中了。然而，要细想一下只剩下32个或更少线程（即一个线程束）的情况。因为线程束的执行是`SIMT（单指令多线程）`的，每条指令之后有隐式的线程束内同步过程。因此，归约循环的最后6个迭代可以用下述语句来展开：

```c
    // unrolling warp
    if (tid < 32)
    {
        volatile int *vmem = idata;
        vmem[tid] += vmem[tid + 32];
        vmem[tid] += vmem[tid + 16];
        vmem[tid] += vmem[tid +  8];
        vmem[tid] += vmem[tid +  4];
        vmem[tid] += vmem[tid +  2];
        vmem[tid] += vmem[tid +  1];
    }
```

这个线程束的展开避免了执行循环控制和线程同步逻辑。注意变量`vmem`是和`volatile`修饰符一起被声明的，它告诉编译器每次赋值时必须将`vmem[tid]`的值存回全局内存中。如果省略了`volatile`修饰符，这段代码将不能正常工作，因为编译器或缓存可能对全局或共享内存优化读写。如果位于全局或共享内存中的变量有`volatile`修饰符，编译器会假定其值可以被其他线程在任何时间修改或使用。因此，任何参考`volatile`修饰符的变量强制直接读或写内存，而不是简单地读写缓存或寄存器。

#### 3.5.3 完全展开的归约

如果编译时已知一个循环中的迭代次数，就可以把循环完全展开。因为在Fermi或Kepler架构中，每个块的最大线程数都是1024，并且在这些归约核函数中循环迭代次数是基于一个线程块维度的，所以完全展开归约循环是可能的。

#### 3.5.4 模板函数的归约

虽然可以手动展开循环，但是使用模板函数有助于进一步减少分支消耗。在设备函数上CUDA支持模板参数。

```c
template <unsigned int iBlockSize>
__global__ void reduceCompleteUnroll(int *g_idata, int *g_odata,
                                     unsigned int n)
{
    // set thread ID
    unsigned int tid = threadIdx.x;
    unsigned int idx = blockIdx.x * blockDim.x * 8 + threadIdx.x;

    // convert global data pointer to the local pointer of this block
    int *idata = g_idata + blockIdx.x * blockDim.x * 8;

    // unrolling 8
    if (idx + 7 * blockDim.x < n)
    {
        int a1 = g_idata[idx];
        int a2 = g_idata[idx + blockDim.x];
        int a3 = g_idata[idx + 2 * blockDim.x];
        int a4 = g_idata[idx + 3 * blockDim.x];
        int b1 = g_idata[idx + 4 * blockDim.x];
        int b2 = g_idata[idx + 5 * blockDim.x];
        int b3 = g_idata[idx + 6 * blockDim.x];
        int b4 = g_idata[idx + 7 * blockDim.x];
        g_idata[idx] = a1 + a2 + a3 + a4 + b1 + b2 + b3 + b4;
    }

    __syncthreads();

    // in-place reduction and complete unroll
    if (iBlockSize >= 1024 && tid < 512) idata[tid] += idata[tid + 512];

    __syncthreads();

    if (iBlockSize >= 512 && tid < 256)  idata[tid] += idata[tid + 256];

    __syncthreads();

    if (iBlockSize >= 256 && tid < 128)  idata[tid] += idata[tid + 128];

    __syncthreads();

    if (iBlockSize >= 128 && tid < 64)   idata[tid] += idata[tid + 64];

    __syncthreads();

    // unrolling warp
    if (tid < 32)
    {
        volatile int *vsmem = idata;
        vsmem[tid] += vsmem[tid + 32];
        vsmem[tid] += vsmem[tid + 16];
        vsmem[tid] += vsmem[tid +  8];
        vsmem[tid] += vsmem[tid +  4];
        vsmem[tid] += vsmem[tid +  2];
        vsmem[tid] += vsmem[tid +  1];
    }


    // write result for this block to global mem
    if (tid == 0) g_odata[blockIdx.x] = idata[0];
}

    // main
    switch (blocksize)
    {
    case 1024:
        reduceCompleteUnroll<1024><<<grid.x / 8, block>>>(d_idata, d_odata,
                size);
        break;

    case 512:
        reduceCompleteUnroll<512><<<grid.x / 8, block>>>(d_idata, d_odata,
                size);
        break;

    case 256:
        reduceCompleteUnroll<256><<<grid.x / 8, block>>>(d_idata, d_odata,
                size);
        break;

    case 128:
        reduceCompleteUnroll<128><<<grid.x / 8, block>>>(d_idata, d_odata,
                size);
        break;

    case 64:
        reduceCompleteUnroll<64><<<grid.x / 8, block>>>(d_idata, d_odata, size);
        break;
    }
```

相比reduceCompleteUnrollWarps8，唯一的区别是使用了模板参数替换了块大小。检查块大小的if语句将在编译时被评估，如果这一条件为false，那么编译时它将会被删除，使得内循环更有效率。例如，在线程块大小为256的情况下调用这个核函数，下述语句将永远是false，编译器会自动从执行内核中移除它。

```c
iBlockSize >= 1024 && tid < 512
```

该核函数一定要在switch-case结构中被调用。这允许编译器为特定的线程块大小自动优化代码，但这也意味着它只对在特定块大小下启动reduceCompleteUnroll有效。

注意，最大的相对性能增益是通过reduceUnrolling8核函数获得的，在这个函数之中每个线程在归约前处理8个数据块。有了8个独立的内存访问，可以更好地让内存带宽饱和及隐藏加载/存储延迟。

### 3.6 动态并行

CUDA的动态并行允许在GPU端直接创建和同步新的GPU内核。在一个核函数中在任意点动态增加GPU应用程序的并行性，是一个令人兴奋的新功能。到目前为止，我们需要把算法设计为单独的、大规模数据并行的内核启动。动态并行提供了一个更有层次结构的方法，在这个方法中，并发性可以在一个GPU内核的多个级别中表现出来。使用动态并行可以让递归算法更加清晰易懂，也更容易理解。

有了动态并行，可以推迟到运行时决定需要在GPU上创建多少个块和网格，可以动态地利用GPU硬件调度器和加载平衡器，并进行调整以适应数据驱动或工作负载。在GPU端直接创建工作的能力可以减少在主机和设备之间传输执行控制和数据的需求，因为在设备上执行的线程可以在运行时决定启动配置

#### 3.6.1 嵌套执行

在动态并行中，内核执行分为两种类型：父母和孩子。父线程、父线程块或父网格启动一个新的网格，即子网格。**子线程、子线程块或子网格被父母启动。子网格必须在父线程、父线程块或父网格完成之前完成。只有在所有的子网格都完成之后，父母才会完成。**

![](/img/post_pics/cuda-c/5.jpeg)

上图说明了父网格和子网格的适用范围。主机线程配置和启动父网格，父网格配置和启动子网格。子网格的调用和完成必须进行适当地嵌套，这意味着在线程创建的所有子网格都完成之后，父网格才会完成。如果调用的线程没有显式地同步启动子网格，那么运行时保证父母和孩子之间的隐式同步。在图中，在父线程中设置了栅栏，从而可以与其子网格显式地同步。

设备线程中的网格启动，在线程块间是可见的。这意味着，线程可能与由该线程启动的或由相同线程块中其他线程启动的子网格同步。在线程块中，只有当所有线程创建的所有子网格完成之后，线程块的执行才会完成。如果块中所有线程在所有的子网格完成之前退出，那么在那些子网格上隐式同步会被触发。

**当父母启动一个子网格，父线程块与孩子显式同步之后，孩子才能开始执行。**

父网格和子网格共享相同的全局和常量内存存储，但它们有不同的局部内存和共享内存。有了孩子和父母之间的弱一致性作为保证，父网格和子网格可以对全局内存并发存取。有两个时刻，子网格和它的父线程见到的内存完全相同：子网格开始时和子网格完成时。当父线程优于子网格调用时，所有的全局内存操作要保证对子网格是可见的。当父母在子网格完成时进行同步操作后，子网格所有的内存操作应保证对父母是可见的。

共享内存和局部内存分别对于线程块或线程来说是私有的，同时，在父母和孩子之间不是可见或一致的。局部内存对线程来说是私有存储，并且对该线程外部不可见。当启动一个子网格时，向局部内存传递一个指针作为参数是无效的。

#### 3.6.2 在GPU上嵌套Hello World

#### 3.6.3 嵌套归约

### 3.7 总结

## 第4章 全局内存

### 4.1 CUDA内存模型概述

#### 4.1.1 内存层次结构的优点

#### 4.1.2 CUDA内存模型

### 4.2 内存管理

#### 4.2.1 内存分配和释放

#### 4.2.2 内存传输

#### 4.2.3 固定内存

#### 4.2.4 零拷贝内存

#### 4.2.5 统一虚拟寻址

#### 4.2.6 统一内存寻址

### 4.3 内存访问模式

#### 4.3.1 对齐与合并访问

#### 4.3.2 全局内存读取

#### 4.3.3 全局内存写入

#### 4.3.4 结构体数组与数组结构体

#### 4.3.5 性能调整

### 4.4 核函数可达到的带宽

#### 4.4.1 内存带宽

#### 4.4.2 矩阵转置问题

### 4.5 使用统一内存的矩阵加法

### 4.6 总结

## 第5章 共享内存和常量内存

### 5.1 CUDA共享内存概述

#### 5.1.1 共享内存

#### 5.1.2 共享内存分配

#### 5.1.3 共享内存存储体和访问模式

#### 5.1.4 配置共享内存量

#### 5.1.5 同步

### 5.2 共享内存的数据布局

#### 5.2.1 方形共享内存

#### 5.2.2 矩形共享内存

### 5.3 减少全局内存访问

#### 5.3.1 使用共享内存的并行归约

#### 5.3.2 使用展开的并行归约

#### 5.3.3 使用动态共享内存的并行归约

#### 5.3.4 有效带宽

### 5.4 合并的全局内存访问

#### 5.4.1 基准转置内核

#### 5.4.2 使用共享内存的矩阵转置

#### 5.4.3 使用填充共享内存的矩阵转置

#### 5.4.4 使用展开的矩阵转置

#### 5.4.5 增大并行性

### 5.5 常量内存

#### 5.5.1 使用常量内存实现一维模板

#### 5.5.2 与只读缓存的比较

### 5.6 线程束洗牌指令

#### 5.6.1 线程束洗牌指令的不同形式

#### 5.6.2 线程束内的共享数据

#### 5.6.3 使用线程束洗牌指令的并行归约

### 5.7 总结

## 第6章 流和并发

### 6.1 流和事件概述

#### 6.1.1 CUDA流

#### 6.1.2 流调度

#### 6.1.3 流的优先级

#### 6.1.4 CUDA事件

#### 6.1.5 流同步

### 6.2 并发内核执行

#### 6.2.1 非空流中的并发内核

#### 6.2.2 Fermi GPU上的虚假依赖关系

#### 6.2.3 使用OpenMP的调度操作

#### 6.2.4 用环境变量调整流行为

#### 6.2.5 GPU资源的并发限制

#### 6.2.6 默认流的阻塞行为

#### 6.2.7 创建流间依赖关系

### 6.3 重叠内核执行和数据传输

#### 6.3.1 使用深度优先调度重叠

#### 6.3.2 使用广度优先调度重叠

### 6.4 重叠GPU和CPU执行

### 6.5 流回调

### 6.6 总结

## 第7章 调整指令级原语

### 7.1 CUDA指令概述

#### 7.1.1 浮点指令

#### 7.1.2 内部函数和标准函数

#### 7.1.3 原子操作指令

### 7.2 程序优化指令

#### 7.2.1 单精度与双精度的比较

#### 7.2.2 标准函数与内部函数的比较

#### 7.2.3 了解原子指令

#### 7.2.4 综合范例

### 7.3 总结

## 第8章 GPU加速库和OpenACC

### 8.1 CUDA库概述

#### 8.1.1 CUDA库支持的作用域

#### 8.1.2 通用的CUDA库工作流

### 8.2 cuSPARSE库

#### 8.2.1 cuSPARSE数据存储格式

#### 8.2.2 用cuSPARSE进行格式转换

#### 8.2.3 cuSPARSE功能示例

#### 8.2.4 cuSPARSE发展中的重要主题

#### 8.2.5 cuSPARSE小结

### 8.3 cuBLAS库

#### 8.3.1 管理cuBLAS数据

#### 8.3.2 cuBLAS功能示例

#### 8.3.3 cuBLAS发展中的重要主题

#### 8.3.4 cuBLAS小结

### 8.4 cuFFT库

#### 8.4.1 使用cuFFT API

#### 8.4.2 cuFFT功能示例

#### 8.4.3 cuFFT小结

### 8.5 cuRAND库 8.5.1 拟随机数或伪随机数的选择

#### 8.5.2 cuRAND库概述

#### 8.5.3 cuRAND介绍

#### 8.5.4 cuRAND发展中的重要主题

### 8.6 CUDA 6.0中函数库的介绍

#### 8.6.1 Drop-In库

#### 8.6.2 多GPU库

### 8.7 CUDA函数库的性能研究

#### 8.7.1 cuSPARSE与MKL的比较

#### 8.7.2 cuBLAS与MKL BLAS的比较

#### 8.7.3 cuFFT与FFTW及MKL的比较

#### 8.7.4 CUDA库性能小结

### 8.8 OpenACC的使用

#### 8.8.1 OpenACC计算指令的使用

#### 8.8.2 OpenACC数据指令的使用

#### 8.8.3 OpenACC运行时API

#### 8.8.4 OpenACC和CUDA库的结合

#### 8.8.5 OpenACC小结

### 8.9 总结

## 第9章 多GPU编程

### 9.1 从一个GPU到多GPU

#### 9.1.1 在多GPU上执行

#### 9.1.2 点对点通信

#### 9.1.3 多GPU间的同步

### 9.2 多GPU间细分计算

#### 9.2.1 在多设备上分配内存

#### 9.2.2 单主机线程分配工作

#### 9.2.3 编译和执行

### 9.3 多GPU上的点对点通信

#### 9.3.1 实现点对点访问

#### 9.3.2 点对点的内存复制

#### 9.3.3 统一虚拟寻址的点对点内存访问

### 9.4 多GPU上的有限差分

#### 9.4.1 二维波动方程的模板计算

#### 9.4.2 多GPU程序的典型模式

#### 9.4.3 多GPU上的二维模板计算

#### 9.4.4 重叠计算与通信

#### 9.4.5 编译和执行

### 9.5 跨GPU集群扩展应用程序

#### 9.5.1 CPU到CPU的数据传输

#### 9.5.2 使用传统MPI在GPU和GPU间传输数据

#### 9.5.3 使用CUDA-aware MPI进行GPU到GPU的数据传输

#### 9.5.4 使用CUDA-aware MPI进行节点内GPU到GPU的数据传输

#### 9.5.5 调整消息块大小

#### 9.5.6 使用GPUDirect RDMA技术进行GPU到GPU的数据传输

### 9.6 总结

## 第10章 程序实现的注意事项

### 10.1 CUDA C的开发过程

#### 10.1.1 APOD开发周期

#### 10.1.2 优化因素

#### 10.1.3 CUDA代码编译

#### 10.1.4 CUDA错误处理

### 10.2 配置文件驱动优化

#### 10.2.1 使用nvprof寻找优化因素

#### 10.2.2 使用nvvp指导优化

#### 10.2.3 NVIDIA工具扩展

### 10.3 CUDA调试

#### 10.3.1 内核调试

#### 10.3.2 内存调试

#### 10.3.3 调试小结

### 10.4 将C程序移植到CUDA C的案例研究

#### 10.4.1 评估crypt

#### 10.4.2 并行crypt

#### 10.4.3 优化crypt

#### 10.4.4 部署crypt

#### 10.4.5 移植crypt小结

### 10.5 总结
