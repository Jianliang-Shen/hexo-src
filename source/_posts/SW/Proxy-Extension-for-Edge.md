---
layout: post
title: Proxy Extension for Edge
index_img: /img/post_pics/proxy/2-1.png
date: 2024-07-17 16:52:28
tags:
    - 网络
categories:
    - 软件
---


在使用多个VPN的情况下有了冲突，可以使用插件的switch功能。

<!-- more -->

- 代理：比如想连外网的VPN
- 直连：比如用于连服务器的VPN

## 找到代理的端口

![](/img/post_pics/proxy/1.png)

## 下载插件

![](/img/post_pics/proxy/2-1.png)

## 设置默认显示

![](/img/post_pics/proxy/3.png)


## 设置代理模式

![](/img/post_pics/proxy/4.png)

## 设置自动切换

下载自动切换配置文件的源(https://github.com/gfwlist/gfwlist) ，包含了大部分外网的信息，需要在代理模式下下载（github链接不稳定）

![](/img/post_pics/proxy/5.png)

设置插件按钮点击所支持的切换模式，上下拖拽：

![](/img/post_pics/proxy/6.png)

点击插件按钮设置为auto switch模式：

![](/img/post_pics/proxy/7.png)

这样就完成了访问的自动切换，当访问至Google时自动切换到代理模式，否则直连（本地为其他vpn假设）