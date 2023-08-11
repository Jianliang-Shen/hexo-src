---
title: Mbedtls学习--对称加密算法
date: 2021-06-02 13:48:26
tags:
    - mbedtls
    - 算法
categories: 
    - 密码学
---


Alice和Bob持有相同的共享秘钥，通过秘钥加解密数据。对称加密算法包括AES、DES和3DES等多种算法。对称加密算法需要对数据进行分组。
<!-- more -->


# 对称加密算法

# 分组密码模式
## ECB Electric Codebook
电子密码本模式，明文与密文一一对应，有明显缺陷。
![](/img/post_pics/mbedtls/2.png)
## CBC Cipher Block Chaining
密码分组链接模式，每一组明文在加密前都与前面的密文分组进行**异或**操作。与第一个分组进行异或的“密文分组”称为初始化向量IV。初始化向量一般由伪随机数生成器派生，IV不可泄露。CBC模式无法抵御选择密文攻击，当密文遭到破坏时，后面的内容无法解密。
![](/img/post_pics/mbedtls/3.png)

## CTR
计数器模式，将累加的计数器与秘钥生成秘钥流，再进行异或。计数器值与分组长度相同，最后一个分组长度不满则截取有效部分。
![](/img/post_pics/mbedtls/4.png)

CTR的优势：
* 可以并行运算
* 可以随机访问，部分密文破坏不影响整体
* 简单，解密等同于加密，无需两套算法
  
# AES
AES是一个对称分组加密算法，分组大小为128bit，密钥长度为128（轮数10）、192（轮数12）、256（轮数14）位。
![](/img/post_pics/mbedtls/5.png)

AES加密过程，
![](/img/post_pics/mbedtls/6.png)


## AES计算过程
* 字节替换--唯一非线性操作
* 行移位
* 列混合
* 轮秘钥加法--异或
* 轮秘钥生成
  
## AES加密工具
### aescrypt2
主要调用`mbedtls_aes_init`，`mbedtls_aes_setkey_enc`，`mbedtls_aes_setkey_dec`等接口，使用方法：
```bash
# mode = 1解密， mode = 0加密
aescrypt2 <mode> <input file> <output file> <hex:key>
```
### crypt_and_hash
支持加密算法和单项散列函数，将结果输出至文件。代码参考`programs/aes/crypt_and_hash.c`
```bash
crypt_and_hash <mode> <input file> <output file> <cipher-alg> <hash-alg> <key>
crypt_and_hash 0 file file.aes AES-128-CBC SHA256 hex:xxxxxxxxxxxxx
```
# 接口与代码示例
## 算法
|算法| 说明 |
|:--|:--|
| AES | 支持ECB、CBC、CTR、CFB和GCM模式 |
| DES | 支持ECB、CBC模式 |
除此之外，还有ARCFOUR(RC4)、Blowfish、Camellia、XTEA等算法。
## 宏定义
|宏定义| 描述 |
|:--|:--|
| MBEDTLS_AES_C |开启AES算法  |
| MBEDTLS_CIPHER_MODE_CBC |开启CBC模式  |
| MBEDTLS_CIPHER_MODE_CTR |开启CTR模式  |
| MBEDTLS_AES_ROM_TABLES |使用预定义S盒（字节替换使用，节省空间）  |
| MBEDTLS_CIPHER_C |开启cipher接口  |
| MBEDTLS_CIPHER_MODE_WITH_PADDING|开启填充  |
| MBEDTLS_CIPHER_PADDING——PKCS7|开启PKCS7填充  |

## 示例代码
接口描述：

|接口| 描述 |
|:--|:--|
| mbedtls_cipher_init         |         初始化cipher结构体  |
| mbedtls_cipher_info_from_string        |         通过算法名称获取cipher信息结构体指针  |
| mbedtls_cipher_setup        |        设置cipher结构体  |
| mbedtls_cipher_get_name         |        获取算法名称  |
| mbedtls_cipher_get_block_size         |         获取算法分组长度  |
| mbedtls_cipher_setkey        |         设置解密密钥接口  |
| mbedtls_cipher_set_iv        |         设置初始化向量接口  |
| mbedtls_cipher_update        |         cipher更新接口  |
| mbedtls_cipher_finish        |         cipher完成接口  |
| mbedtls_cipher_free        |         释放cipher结构体  |

代码运行中需要提供明文、密钥和初始化向量。
### main.c

```c
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "mbedtls/cipher.h"
#include "mbedtls/platform.h"

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
    0x51, 0x2e, 0x03, 0xd5, 0x34, 0x12, 0x00, 0x06
};

uint8_t iv[16] =
{
    0x3d, 0xaf, 0xba, 0x42, 0x9d, 0x9e, 0xb4, 0x30,
    0xb4, 0x22, 0xda, 0x80, 0x2c, 0x9f, 0xac, 0x41
};

static void dump_buf(char *info, uint8_t *buf, uint32_t len)
{
    mbedtls_printf("%s", info);
    for (int i = 0; i < len; i++) {
        mbedtls_printf("%s%02X%s", i % 16 == 0 ? "\n\t":" ", 
                        buf[i], i == len - 1 ? "\n":"");
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
    mbedtls_printf("\n  cipher info setup, name: %s, block size: %d\n", 
                        mbedtls_cipher_get_name(&ctx), 
                        mbedtls_cipher_get_block_size(&ctx));

    mbedtls_cipher_setkey(&ctx, key, sizeof(key)*8, MBEDTLS_ENCRYPT);
    mbedtls_cipher_set_iv(&ctx, iv, sizeof(iv));
    mbedtls_cipher_update(&ctx, ptx, strlen(ptx), buf, &len);
    olen += len;

    mbedtls_cipher_finish(&ctx, buf + len, &len);
    olen += len;

    dump_buf("\n  cipher aes encrypt:", buf, olen);

    mbedtls_cipher_free(&ctx);
}

int main(void)
{
    cipher(MBEDTLS_CIPHER_AES_128_CBC);
    cipher(MBEDTLS_CIPHER_AES_128_CTR);

    return 0;
}

```
与单向散列函数计算过程相似，这里使用setkey和setiv代替start，而update可以多次调用形成数据流，在目标板内存受限时减小资源的压力，finish函数则可以结束运算。
### mbedtls_config.h

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
#define MBEDTLS_AES_C
#define MBEDTLS_CIPHER_C
#define MBEDTLS_CIPHER_MODE_CBC
#define MBEDTLS_CIPHER_MODE_CTR
#define MBEDTLS_CIPHER_MODE_WITH_PADDING
#define MBEDTLS_CIPHER_PADDING_PKCS7

#define MBEDTLS_AES_ROM_TABLES

#include "mbedtls/check_config.h"

#endif /* MBEDTLS_CONFIG_H */

```

### CMakeLists.txt

```bash
cmake_minimum_required(VERSION 3.8)

project("aes")

include_directories(./ $ENV{MBEDTLS_BASE}/include)
aux_source_directory($ENV{MBEDTLS_BASE}/library MBEDTLS_SOURCES)


set(SOURCES 
	${CMAKE_CURRENT_LIST_DIR}/main.c
    ${CMAKE_CURRENT_LIST_DIR}/mbedtls_config.h 
	${MBEDTLS_SOURCES})

add_executable(aes ${SOURCES})
```
运行结果：

```bash
mkdir -p build && cd build && cmake .. && make -j32
./aes               

  cipher info setup, name: AES-128-CBC, block size: 16

  cipher aes encrypt:
        4D DF 90 12 D7 B3 89 87 45 A1 ED 98 60 EB 0F A2
        FD 2B BD 80 D2 71 90 D7 2A 2F 24 0C 8F 37 2A 27
        63 74 62 96 DD C2 BF CE 7C 25 2B 6C D7 DD 4B A8
        57 7E 09 6D BD 80 24 C8 B4 C5 A1 16 0C A2 D3 F9


  cipher info setup, name: AES-128-CTR, block size: 16

  cipher aes encrypt:
        C4 1A 1D B1 56 C0 9B 59 E8 25 D9 5B 72 FD 97 BE
        F7 06 BA C1 B8 4F F5 4E 72 88 2D 17 0B DB 53 0A
        9B 0A FD 86 41 65 73 06 6B C1 F0 52 18 FC 1D 57
        9D F4 81 F7 08 CB
```

### cipher相关结构体

```c
typedef struct mbedtls_cipher_context_t
{
    /** Information about the associated cipher. */
    const mbedtls_cipher_info_t *cipher_info;

    /** Key length to use. */
    int key_bitlen;

    /** Operation that the key of the context has been
     * initialized for.
     */
    mbedtls_operation_t operation;

#if defined(MBEDTLS_CIPHER_MODE_WITH_PADDING)
    /** Padding functions to use, if relevant for
     * the specific cipher mode.
     */
    void (*add_padding)( unsigned char *output, size_t olen, size_t data_len );
    int (*get_padding)( unsigned char *input, size_t ilen, size_t *data_len );
#endif

    /** Buffer for input that has not been processed yet. */
    unsigned char unprocessed_data[MBEDTLS_MAX_BLOCK_LENGTH];

    /** Number of Bytes that have not been processed yet. */
    size_t unprocessed_len;

    /** Current IV or NONCE_COUNTER for CTR-mode, data unit (or sector) number
     * for XTS-mode. */
    unsigned char iv[MBEDTLS_MAX_IV_LENGTH];

    /** IV size in Bytes, for ciphers with variable-length IVs. */
    size_t iv_size;

    /** The cipher-specific context. */
    void *cipher_ctx;

#if defined(MBEDTLS_CMAC_C)
    /** CMAC-specific context. */
    mbedtls_cmac_context_t *cmac_ctx;
#endif

#if defined(MBEDTLS_USE_PSA_CRYPTO)
    /** Indicates whether the cipher operations should be performed
     *  by Mbed TLS' own crypto library or an external implementation
     *  of the PSA Crypto API.
     *  This is unset if the cipher context was established through
     *  mbedtls_cipher_setup(), and set if it was established through
     *  mbedtls_cipher_setup_psa().
     */
    unsigned char psa_enabled;
#endif /* MBEDTLS_USE_PSA_CRYPTO */

} mbedtls_cipher_context_t;
```

```c
typedef struct mbedtls_cipher_info_t
{
    /** Full cipher identifier. For example,
     * MBEDTLS_CIPHER_AES_256_CBC.
     */
    mbedtls_cipher_type_t type;

    /** The cipher mode. For example, MBEDTLS_MODE_CBC. */
    mbedtls_cipher_mode_t mode;

    /** The cipher key length, in bits. This is the
     * default length for variable sized ciphers.
     * Includes parity bits for ciphers like DES.
     */
    unsigned int key_bitlen;

    /** Name of the cipher. */
    const char * name;

    /** IV or nonce size, in Bytes.
     * For ciphers that accept variable IV sizes,
     * this is the recommended size.
     */
    unsigned int iv_size;

    /** Bitflag comprised of MBEDTLS_CIPHER_VARIABLE_IV_LEN and
     *  MBEDTLS_CIPHER_VARIABLE_KEY_LEN indicating whether the
     *  cipher supports variable IV or variable key sizes, respectively.
     */
    int flags;

    /** The block size, in Bytes. */
    unsigned int block_size;

    /** Struct for base cipher information and functions. */
    const mbedtls_cipher_base_t *base;

} mbedtls_cipher_info_t;
```
