---
title: Mbedtls学习--移植库至K210平台
date: 2021-05-22 13:50:21
index_img: /img/post_pics/index_img/mbed.png
tags:
    - mbedtls
    - 算法
categories: 
    - 密码学
---


# K210 RISC-V
亚博智能开发板，使用的编译工具和sdk为riscv64-unknown-elf-gcc和kendryte-standalone-sdk-develop，开发环境为windows。

<!-- more --> 

![](/img/post_pics/index_img/mbed.png)

# 移植过程简述
* 在sdk/lib下添加mbedtls的文件夹，删除无关的文件只保留.c和.h文件：
	* include
	* library
* 在sdk/lib/mbedtls下添加CMakeLists.txt

```bash
FILE(GLOB_RECURSE MBEDTLS_SRC
        "${CMAKE_CURRENT_LIST_DIR}/include/mbedtls/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/include/mbedtls/*.h"
        "${CMAKE_CURRENT_LIST_DIR}/include/psa/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/include/psa/*.h"
        "${CMAKE_CURRENT_LIST_DIR}/library/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/library/*.h"
)

ADD_LIBRARY(mbedtls
        ${MBEDTLS_SRC}
)
TARGET_COMPILE_OPTIONS(mbedtls PRIVATE -O2)
```
* 在lib/CMakeLists.txt中添加

```bash
ADD_SUBDIRECTORY(mbedtls)
ADD_LIBRARY(kendryte
        ${LIB_SRC}
        ${MBEDTLS_SRC}
)

TARGET_LINK_LIBRARIES(kendryte
        PUBLIC
                nncase
                mbedtls
)
```
* 全局搜索替换
	* `#include "mbedtls/`替换为`#include "`
	* `#include <mbedtls/`替换为`#include <`
	* `#include "psa/`替换为`#include "`
	* `#include <psa/`替换为`#include <`
* 重命名mbedtls中的
	* `aes.h`重命名`mbed_aes.h`
	* `platform.h`重命名`mbed_platform.h`
	* `sha256.h`重命名`mbed_sha256.h`
并将mbedtls中依赖这些头文件的代码替换成新名称。
* 设置mbedtls/config.h中的几个宏，这是平台的限制：

```c
#define MBEDTLS_NO_PLATFORM_ENTROPY
// #define MBEDTLS_NET_C
// #define MBEDTLS_TIMING_C
```
* 由于库的冲突，删除x509_crl.c和x509_crt.c文件
  
# 测试例程
使用IOT-Security仓库的AES代码，在sdk/src下添加test_mbedtls文件夹，下面新建main.c和pin_config.h文件，代码如下：
main.c

```c
#include <stdio.h>
#include <string.h>
#include "pin_config.h"

#include <stdint.h>
#include "cipher.h"
#include "mbed_platform.h"

/*
    # padding with pkcs7 AES_128_CBC Encrypt
    ptx = "CBC has been the most commonly used mode of operation."
    key = 06a9214036b8a15b512e03d534120006
    iv  = 3dafba429d9eb430b422da802c9fac41
    ctx = 4DDF9012D7B3898745A1ED9860EB0FA2
          FD2BBD80D27190D72A2F240C8F372A27
          63746296DDC2BFCE7C252B6CD7DD4BA8
          577E096DBD8024C8B4C5A1160CA2D3F9
*/
char *ptx = "CBC has been the most commonly used mode of operation.";
uint8_t key[16] =
    {
        0x06, 0xa9, 0x21, 0x40, 0x36, 0xb8, 0xa1, 0x5b,
        0x51, 0x2e, 0x03, 0xd5, 0x34, 0x12, 0x00, 0x06};

uint8_t iv[16] =
    {
        0x3d, 0xaf, 0xba, 0x42, 0x9d, 0x9e, 0xb4, 0x30,
        0xb4, 0x22, 0xda, 0x80, 0x2c, 0x9f, 0xac, 0x41};

static void dump_buf(char *info, uint8_t *buf, uint32_t len)
{
    mbedtls_printf("%s", info);
    for(int i = 0; i < len; i++)
    {
        mbedtls_printf("%s%02X%s", i % 16 == 0 ? "\n\t" : " ",
                       buf[i], i == len - 1 ? "\n" : "");
    }
    mbedtls_printf("\n");
}

void cipher(int type)
{
    size_t len;
    int olen = 0;
    uint8_t buf[64];

    mbedtls_cipher_context_t ctx;
    const mbedtls_cipher_info_t *info;

    mbedtls_cipher_init(&ctx);
    info = mbedtls_cipher_info_from_type(type);

    mbedtls_cipher_setup(&ctx, info);
    mbedtls_printf("\ncipher info setup, name: %s, block size: %d\n",
                   mbedtls_cipher_get_name(&ctx),
                   mbedtls_cipher_get_block_size(&ctx));

    mbedtls_cipher_setkey(&ctx, key, sizeof(key) * 8, MBEDTLS_ENCRYPT);
    mbedtls_cipher_set_iv(&ctx, iv, sizeof(iv));
    mbedtls_cipher_update(&ctx, ptx, strlen(ptx), buf, &len);
    olen += len;

    mbedtls_cipher_finish(&ctx, buf + len, &len);
    olen += len;

    dump_buf("\ncipher aes encrypt:", buf, olen);

    mbedtls_cipher_free(&ctx);
}

void hardware_init(void)
{
    // fpioa映射
    fpioa_set_function(PIN_UART_USB_RX, FUNC_UART_USB_RX);
    fpioa_set_function(PIN_UART_USB_TX, FUNC_UART_USB_TX);
}

int main(void)
{
    hardware_init();
    // 初始化串口3，设置波特率为115200
    uart_init(UART_USB_NUM);
    uart_configure(UART_USB_NUM, 115200, UART_BITWIDTH_8BIT, UART_STOP_1, UART_PARITY_NONE);

    char *info = {"This is AES test on K210 RISC-V!\n"};
    uart_send_data(UART_USB_NUM, info, strlen(info));

    cipher(MBEDTLS_CIPHER_AES_128_CBC);
    cipher(MBEDTLS_CIPHER_AES_128_CTR);

    while(1)
    {
    }
    return 0;
}

```
pin_config.h

```c
#ifndef _PIN_CONFIG_H_
#define _PIN_CONFIG_H_
/*****************************HEAR-FILE************************************/
#include "fpioa.h"
#include "uart.h"

/*****************************HARDWARE-PIN*********************************/
// 硬件IO口，与原理图对应
#define PIN_UART_USB_RX       (4)
#define PIN_UART_USB_TX       (5)

/*****************************SOFTWARE-GPIO********************************/
// 软件GPIO口，与程序对应
#define UART_USB_NUM           UART_DEVICE_3

/*****************************FUNC-GPIO************************************/
// GPIO口的功能，绑定到硬件IO口
#define FUNC_UART_USB_RX       (FUNC_UART1_RX + UART_USB_NUM * 2)
#define FUNC_UART_USB_TX       (FUNC_UART1_TX + UART_USB_NUM * 2)

#endif /* _PIN_CONFIG_H_ */

```
在sdk下新建build文件夹并进入：

```bash
cmake .. -DPROJ=test_mbedtls -G "MinGW Makefiles"
```
将编译好的文件烧入开发板中
![](/img/post_pics/mbedtls/11.png)
上电运行：
![](/img/post_pics/mbedtls/12.png)
demo运行成功。