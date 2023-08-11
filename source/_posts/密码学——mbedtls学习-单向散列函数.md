---
title: Mbedtls学习--单向散列函数
date: 2021-06-01 13:47:56
tags:
    - mbedtls
    - 算法
categories: 
    - 密码学
---

单向散列函数原理及应用。

<!-- more -->

## 单向散列函数
### 原理
满足密码学算法安全属性的特殊散列函数，保证数据的完整性，抵御篡改攻击，原数据称作**消息**，计算后的输出称为**摘要**，在通信中，Bob接受Alice发来的消息及摘要，通过相同的算法计算摘要比对以验证消息是否被篡改。

<!-- more -->

### 性质
输入长度可变，输出长度固定，计算过程应当高效率，具备单向性。
* 抗弱碰撞性，对于给定$a$，能找到$b$使得$h(a)=h(b)$不可行
* 抗强碰撞性，给定一个输出$z$，能够找到$h(a)=z$不可行
  
### 应用
消息完整性检测、伪随机数生成器、数字签名、消息认证码

## 单向散列函数实现方法
### MD算法
* MD4：散列碰撞已被攻破
* MD5：抗碰撞性已被攻破
  
### SHA算法
SHA0/SHA1不再使用
* SHA2：包括SHA256，SHA384，SHA512
* SHA3：更为安全
  
## SHA256
处理步骤：
* 预处理
	* 消息填充
	* 消息分割
	* 设置初始摘要值 $H^{(0)}$
	* 准备常量$K^{256}_{i}$
* 哈希计算
	* 消息调度
	* 初始化工作寄存器
	* 更新工作寄存器
	* 计算消息摘要
  
## Mbedtls单项散列函数
### generic_sum
计算散列值
```bash
generic_sum SHA256 <file name>
```
### 头文件和函数接口

```c
#include "mbedtls/md.h"
```
|接口|描述  |
|:--|:--|
| mbedtls_md_init |初始化md结构体  |
| mbedtls_md_info_from_type |根据算法类型得到md信息结构体指针  |
| mbedtls_md_setup| 初始化md结构体  |
| mbedtls_md_get_name |获得单向散列算法名称  |
| mbedtls_md_get_size |获取单向散列算法输出消息摘要长度  |
| mbedtls_md_starts |md启动接口  |
| mbedtls_md_update| md更新接口，处理输入数据，包括预处理和计算  |
| mbedtls_md_finish |md输出消息摘要结果  |
| mbedtls_md_free |释放md结构体  |

### 依赖
#define MBEDTLS_MD_C
#define MBEDTLS_SHA256_C

### 示例代码

```c
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "mbedtls/md.h"
#include "mbedtls/platform.h"

static void dump_buf(char *info, uint8_t *buf, uint32_t len)
{
    mbedtls_printf("%s", info);
    for (int i = 0; i < len; i++) {
        mbedtls_printf("%s%02X%s", i % 16 == 0 ? "\n\t":" ", 
                        buf[i], i == len - 1 ? "\n":"");
    }
    mbedtls_printf("\n");
}

int main(void)
{
    uint8_t digest[32];
    char *msg = "abc";

    mbedtls_md_context_t ctx;
    const mbedtls_md_info_t *info;

    mbedtls_md_init(&ctx);
    info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);

    mbedtls_md_setup(&ctx, info, 0);
    mbedtls_printf("\n  md info setup, name: %s, digest size: %d\n", 
                   mbedtls_md_get_name(info), mbedtls_md_get_size(info));

    mbedtls_md_starts(&ctx);
    mbedtls_md_update(&ctx, msg, strlen(msg));
    mbedtls_md_finish(&ctx, digest);

    dump_buf("\n  md sha-256 digest:", digest, sizeof(digest));

    mbedtls_md_free(&ctx); 

    return 0;
}
```

config头文件

```c
#ifndef MBEDTLS_CONFIG_H
#define MBEDTLS_CONFIG_H

/* System support */
#define MBEDTLS_PLATFORM_C
#define MBEDTLS_PLATFORM_MEMORY
#define MBEDTLS_MEMORY_BUFFER_ALLOC_C
#define MBEDTLS_PLATFORM_NO_STD_FUNCTIONS
#define MBEDTLS_PLATFORM_EXIT_ALT
#define MBEDTLS_NO_PLATFORM_ENTROPY
#define MBEDTLS_NO_DEFAULT_ENTROPY_SOURCES
#define MBEDTLS_PLATFORM_PRINTF_ALT

/* mbed TLS modules */
#define MBEDTLS_MD_C
#define MBEDTLS_SHA256_C

#include "mbedtls/check_config.h"

#endif /* MBEDTLS_CONFIG_H */
```
CMakeLists.txt

```bash
cmake_minimum_required(VERSION 3.8)

project("hash")

include_directories(./ $ENV{MBEDTLS_BASE}/include)
aux_source_directory($ENV{MBEDTLS_BASE}/library MBEDTLS_SOURCES)


set(SOURCES 
	${CMAKE_CURRENT_LIST_DIR}/main.c
    ${CMAKE_CURRENT_LIST_DIR}/mbedtls_config.h 
	${MBEDTLS_SOURCES})

add_executable(hash ${SOURCES})
```
运行结果：

```bash
mkdir -p build && cd build && cmake .. && make -j32
./hash    

  md info setup, name: SHA256, digest size: 32

  md sha-256 digest:
        BA 78 16 BF 8F 01 CF EA 41 41 40 DE 5D AE 22 23
        B0 03 61 A3 96 17 7A 9C B4 10 FF 61 F2 00 15 AD
```
实际应用中，`mbedtls_md_update`可以调用多次，不断填充数据。