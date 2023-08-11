---
title: Mbedtls学习--消息认证码
date: 2021-06-13 13:50:52
tags:
    - mbedtls
    - 算法
categories: 
    - 密码学
---

MAC，Message Authentication Code，帮助接收者判断消息是否被第三方篡改，确保消息的完整性和真实性。常使用单向散列函数实现，称为HMAC-SHA1和HMAC-SHA256。也可以使用分组加密构建，称为CMAC、GCM和CCM。MAC和Hash的不同在于输入多一个密钥。

<!-- more --> 

# 消息认证码


![](/img/post_pics/mbedtls/7.png)

# 实现形式
* HMAC：HMAC是基于单向散列函数的算法
* CBC-MAC和CMAC
* 认证加密CCM，输入包括明文、一次性整数Nonce、相关数据A和密钥K，输出密文C和认证码T，使用的算法是CBC-MAC
  
![](/img/post_pics/mbedtls/8.png)

* 认证加密GCM：输入包括明文、初始化向量IV、相关数据A和密钥K，输出密文C和认证码T，使用的算法是CBC-MAC
  
![](/img/post_pics/mbedtls/9.png)

# HMAC示例

## 依赖的宏
MBEDTLS_MD_C
MBEDTLS_SHA256_C
## 函数接口
|**接口**|**描述**|
|:--|:--|
| mbedtls_md_hmac_starts        |         启动HMAC计算  |
| mbedtls_md_hmac_update        |         更新输入数据  |
| mbedtls_md_hmac_finish        |        完成计算  |

## 代码示例
运行平台K210，移植参考：[mbedtls学习--移植库至K210平台](https://blog.csdn.net/JackSparrow_sjl/article/details/118583913)
```c

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pin_config.h"

#include "mbed_platform.h"
#include "md.h"

void hardware_init(void)
{
    // fpioa映射
    fpioa_set_function(PIN_UART_USB_RX, FUNC_UART_USB_RX);
    fpioa_set_function(PIN_UART_USB_TX, FUNC_UART_USB_TX);
}
//************************* Mbedtls code start *****************************

static void dump_buf(char *info, uint8_t *buf, uint32_t len)
{
    mbedtls_printf("%s", info);
    for(int i = 0; i < len; i++)
    {
        mbedtls_printf("%s%02X%s", i % 16 == 0 ? "\n     " : " ",
                       buf[i], i == len - 1 ? "\n" : "");
    }
}

//************************* Mbedtls code end *******************************

int main(void)
{
    hardware_init();
    // 初始化串口3，设置波特率为115200
    uart_init(UART_USB_NUM);
    uart_configure(UART_USB_NUM, 115200, UART_BITWIDTH_8BIT, UART_STOP_1, UART_PARITY_NONE);

    /* 开机发送hello yahboom! */
    char *log = {"Run mbedtls on K210\n"};
    uart_send_data(UART_USB_NUM, log, strlen(log));

    //******************************* mbedtls code start *******************

    uint8_t mac[32];
    char *secret = "Jefe";
    char *msg = "what do ya want for nothing?";

    mbedtls_md_context_t ctx;
    const mbedtls_md_info_t *info;

    mbedtls_md_init(&ctx);
    info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);

    mbedtls_md_setup(&ctx, info, 1);
    mbedtls_printf("\n  md info setup, name: %s, digest size: %d\n",
                   mbedtls_md_get_name(info), mbedtls_md_get_size(info));

    mbedtls_md_hmac_starts(&ctx, secret, strlen(secret));
    mbedtls_md_hmac_update(&ctx, msg, strlen(msg));
    mbedtls_md_hmac_finish(&ctx, mac);

    dump_buf("\n  md hmac-sha-256 mac:", mac, sizeof(mac));

    mbedtls_md_free(&ctx);

    //******************************* mbedtls code end **********************

    while(1)
    {
    }
    return 0;
}

```
运行结果：
![](/img/post_pics/mbedtls/10.png)
