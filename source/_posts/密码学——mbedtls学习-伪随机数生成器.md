---
title: Mbedtls学习--伪随机数生成器
date: 2021-06-15 13:51:25
tags:
    - mbedtls
    - 算法
categories: 
    - 密码学
---

伪随机数应用和原理。

<!-- more -->

# 伪随机数
## 应用
* 生成盐，用于基于口令的密码
* 生成密钥，用于加密和认证
* 生成一次性整数Nonce，防止重放攻击
* 生成初始化向量IV
  
<!-- more --> 
 
## 构成
* 种子，真随机数生成器的种子来源于物理现象
* 内部状态，种子用来初始化内部状态
  
## 算法
|算法|原理|
|:--|:--|
|Hash_DRBG|使用单向散列函数|
|HMAC_DRBG|使用消息认证码|
|CTR_DRBG| 使用分组密码算法|
## 工具

```bash
gen_entropy file    		# 产生熵
gen_random_ctr_drbg file 	# 生成随机数
gen_random_havege file 		# 使用硬件时钟源生成伪随机数，依赖MBEDTLS_HAVEGE_C
```

# CTR_DRBG
## 原理
![](/img/post_pics/mbedtls/1.png)
攻击时，由于无法破解明文获取内部状态，得到计数值，因此无法预测下一个随机数。
