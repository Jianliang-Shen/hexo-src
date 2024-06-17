---
layout: post
title: WSL NV相关环境配置
date: 2024-06-16 23:46:14
index_img: /img/post_pics/ai/wsl.png
tags:
    - 算法
    - AI
categories: 
    - GPU
---

在RTX4070S windows中配置WSL相关的AI环境，包括CUDA，PyTorch，Cudnn等
<!-- more -->

## Cuda

安装WSL/Docker/Nvidia：
[Windows 下让 Docker Desktop 关联上 NVidia GPU](https://blog.csdn.net/ndscvipuser/article/details/136610169)
[如何查看wsl是wsl1还是wsl2](https://blog.csdn.net/dghcs18/article/details/134244426)
[Nvidia WSL官方指引](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/latest/install-guide.html)
[linux上cuda相关包与opencv及相关模块安装（wsl+ubuntu22.04）](https://blog.csdn.net/weixin_55274216/article/details/137630257)

>注意：WSL不需要装cuda驱动，丢在win host安装，比如Geforce等驱动软件，安装完成后运行`nvidia-smi`

```bash
# Powershell中测试
docker run --rm --runtime=nvidia --gpus all ubuntu nvidia-smi
```

Docker Desktop中的设置：

![1](/img/post_pics/ai/docker_set1.png)
![2](/img/post_pics/ai/docker_set2.png)

测试结果：

![3](/img/post_pics/ai/docker.png)

WSL中测试：

![4](/img/post_pics/ai/wsl.png)

安装[cuda toolkit](https://developer.nvidia.cn/cuda-downloads)

```bash
wget https://developer.download.nvidia.com/compute/cuda/repos/wsl-ubuntu/x86_64/cuda-wsl-ubuntu.pin
sudo mv cuda-wsl-ubuntu.pin /etc/apt/preferences.d/cuda-repository-pin-600
wget https://developer.download.nvidia.com/compute/cuda/12.4.1/local_installers/cuda-repo-wsl-ubuntu-12-4-local_12.4.1-1_amd64.deb
sudo dpkg -i cuda-repo-wsl-ubuntu-12-4-local_12.4.1-1_amd64.deb
sudo cp /var/cuda-repo-wsl-ubuntu-12-4-local/cuda-*-keyring.gpg /usr/share/keyrings/
sudo apt-get update
sudo apt-get -y install cuda-toolkit-12-4

# ~/.bashrc
export LD_LIBRARY_PATH=/usr/local/cuda/lib64
export PATH=$PATH:/usr/local/cuda/bin

nvcc -V
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2024 NVIDIA Corporation
Built on Thu_Mar_28_02:18:24_PDT_2024
Cuda compilation tools, release 12.4, V12.4.131
Build cuda_12.4.r12.4/compiler.34097967_0
```

## Conda

```bash
#创建环境
conda init
conda create -n torch-gpu  python=3.9
conda activate torch-gpu

#换源，参考https://blog.csdn.net/watermelon1123/article/details/88122020
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/
conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main/
conda config --set show_channel_urls yes

#安装torch，命令在 https://pytorch.org/ 寻找
conda install pytorch torchvision torchaudio pytorch-cuda=12.1 -c pytorch-nightly -c nvidia

# ~/.bashrc
conda activate torch-gpu
```

## cuDNN

下载安装方法：https://developer.nvidia.com/cudnn-downloads

```bash
wget https://developer.download.nvidia.com/compute/cudnn/9.2.0/local_installers/cudnn-local-repo-ubuntu2004-9.2.0_1.0-1_amd64.deb
sudo dpkg -i cudnn-local-repo-ubuntu2004-9.2.0_1.0-1_amd64.deb
sudo cp /var/cudnn-local-repo-ubuntu2004-9.2.0/cudnn-*-keyring.gpg /usr/share/keyrings/
sudo apt-get update
sudo apt-get -y install cudnn-cuda-12

# 查看版本
cat /usr/include/cudnn_version.h | grep CUDNN_MAJOR -A 2
```
