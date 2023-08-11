---
title: Mbedtls学习--大数运算
date: 2021-05-31 13:47:15
tags:
    - mbedtls
    - 算法
categories: 
    - 密码学
---


大数计算，顾名思义，指超出64位的数的乘法运算、指数运算和模逆运算，其中模逆运算，特指求逆元，所谓乘法逆元，例如：
$$
2*9 mod 17 = 1
$$
则9是2关于模17的逆元（余数为1的被除数）或者2 * 9 与 1 关于模17同余即：
$$
9 = 2^{-1} mod 17
$$

<!-- more -->
  
## 库文件
mbedtls/library/bignum.c
## 依赖宏
MBEDTLS_BIGNUM_C
MBEDTLS_PLATFORM_C
## 接口
|接口|描述  |
|:--|:--|
| mbedtls_mpi_init |初始化大数结构体  |
| mbedtls_mpi_read_string |读取字符串到大数结构体  |
| mbedtls_mpi_write_string |大数结构体输出到字符串  |
| mbedtls_mpi_mul_mpi |大数乘法  |
| mbedtls_mpi_exp_mod |大数指数  |
| mbedtls_mpi_inv_mod |大数模逆  |
| mbedtls_mpi_free |释放大数结构体  |

## 示例代码
代码结构

```bash
.
├── build
├── CMakeLists.txt
├── main.c
└── mbedtls_config.h    #对使用的库的裁剪，包含需要的库
```
main.c
```c
#include <string.h>
#include <stdio.h>

#include "mbedtls/bignum.h"
#include "mbedtls/platform.h"

static void dump_buf(char *buf, size_t len) 
{
    for (int i = 0; i < len; i++) {
        mbedtls_printf("%c%s", buf[i], 
                        (i + 1) % 32 ? "" : "\n\t"); 
    }
    mbedtls_printf("\n");
}

int main(void)
{
    size_t olen;
    char buf[256];
    mbedtls_mpi A, E, N, X;

    mbedtls_mpi_init(&A); 
    mbedtls_mpi_init(&E); 
    mbedtls_mpi_init(&N); 
    mbedtls_mpi_init(&X);

    mbedtls_mpi_read_string(&A, 16,					//以16进制读取字符串，允许2-16进制
        "EFE021C2645FD1DC586E69184AF4A31E" \
        "D5F53E93B5F123FA41680867BA110131" \
        "944FE7952E2517337780CB0DB80E61AA" \
        "E7C8DDC6C5C6AADEB34EB38A2F40D5E6" );

    mbedtls_mpi_read_string(&E, 16,
        "B2E7EFD37075B9F03FF989C7C5051C20" \
        "34D2A323810251127E7BF8625A4F49A5" \
        "F3E27F4DA8BD59C47D6DAABA4C8127BD" \
        "5B5C25763222FEFCCFC38B832366C29E" );

    mbedtls_mpi_read_string(&N, 16,
        "0066A198186C18C10B2F5ED9B522752A" \
        "9830B69916E535C8F047518A889A43A5" \
        "94B6BED27A168D31D4A52F88925AA8F5" );

    mbedtls_mpi_mul_mpi(&X, &A, &N);
    mbedtls_mpi_write_string(&X, 16, buf, 256, &olen);
    mbedtls_printf("\n  X = A * N = \n\t");
    dump_buf(buf, olen);

    mbedtls_mpi_exp_mod(&X, &A, &E, &N, NULL);
    mbedtls_mpi_write_string(&X, 16, buf, 256, &olen);
    mbedtls_printf("\n  X = A^E mode N = \n\t");
    dump_buf(buf, olen);

    mbedtls_mpi_inv_mod( &X, &A, &N);
    mbedtls_mpi_write_string(&X, 16, buf, 256, &olen);
    mbedtls_printf("\n  X = A^-1 mod N = \n\t");
    dump_buf(buf, olen);

    mbedtls_mpi_free(&A); 
    mbedtls_mpi_free(&E);
    mbedtls_mpi_free(&N); 
    mbedtls_mpi_free(&X);

    return 0;   
}
```

CmakeList.txt

```bash
cmake_minimum_required(VERSION 3.8)

project("bignum")

include_directories(./ $ENV{MBEDTLS_BASE}/include)
aux_source_directory($ENV{MBEDTLS_BASE}/library MBEDTLS_SOURCES)

set(SOURCES 
	${CMAKE_CURRENT_LIST_DIR}/main.c
    ${CMAKE_CURRENT_LIST_DIR}/mbedtls_config.h 
	${MBEDTLS_SOURCES})

add_executable(bignum ${SOURCES})
```
mbedtls_config.h

```c
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
#define MBEDTLS_BIGNUM_C

#include "mbedtls/check_config.h"

#endif /* MBEDTLS_CONFIG_H */

```
运行测试

```bash
mkdir -p build && cd build && cmake .. && make -j32
./bignum          

  X = A * N = 
        602AB7ECA597A3D6B56FF9829A5E8B85
        9E857EA95A03512E2BAE7391688D264A
        A5663B0341DB9CCFD2C4C5F421FEC814
        8001B72E848A38CAE1C65F78E56ABDEF
        E12D3C039B8A02D6BE593F0BBBDA56F1
        ECF677152EF804370C1A305CAF3B5BF1
        30879B56C61DE584A0F53A2447A51E

  X = A^E mode N = 
        36E139AEA55215609D2816998ED020BB
        BD96C37890F65171D948E9BC7CBAA4D9
        325D24D6A3C12710F10A09FA08AB87

  X = A^-1 mod N = 
        3A0AAEDD7E784FC07D8F9EC6E3BFD5C3
        DBA76456363A10869622EAC2DD84ECC5
        B8A74DAC4D09E03B5E0BE779F2DF61
```
## 算法分析

```c
typedef struct mbedtls_mpi
{
    int s;              			/*!<  Sign: -1 if the mpi is negative, 1 otherwise */
    size_t n;           			/*!<  total # of limbs  */
    mbedtls_mpi_uint *p;          	/*!<  pointer to limbs  */
}
mbedtls_mpi;
```
X->s指大数的正负，X->指数位总数，X->p[n]指向每一位，大小为32位或64位数。例如十六进制数-1 000000000000000F的结果为，最低位为65535，最高位为1，这里每16个字符是一位。

```c
A->s = -1 , A->n = 2
A->[0] = 65535
A->[1] = 1
```

### 数位统计

```c
size_t mbedtls_mpi_bitlen( const mbedtls_mpi *X )
```
该函数计算大数的bit位的总个数，忽略前导0，譬如十六进制`-1000000000000FFFF`的运算结果为65，实现过程如下：i从最高位`n-1`往后数，遇到`X->p[i] != 0`截止，例如`-1000000000000FFFF`的n=2, i=1开始，此时再计算当前位（最左位）的bit数，依靠

```c
static size_t mbedtls_clz( const mbedtls_mpi_uint x )
```
函数来计算，这个函数将数值转为二进制并去除前导0计算剩余的有效bit位的总和，返回j=1，则大数的数位综合为`( i * biL ) + j`，biL在这里值为64。

### 读取字符串

```c
int mbedtls_mpi_read_string( mbedtls_mpi *X, int radix, const char *s )
```
radix指进制，支持2-16进制以内的读取。以十六进制为例：

```c
    mbedtls_mpi_init( &T );		//初始化一个大数
    slen = strlen( s );         //获取字符串长度
    if( radix == 16 )
    {
        if( slen > MPI_SIZE_T_MAX >> 2 )					//判断是否超出范围
            return( MBEDTLS_ERR_MPI_BAD_INPUT_DATA );

        n = BITS_TO_LIMBS( slen << 2 );

        MBEDTLS_MPI_CHK( mbedtls_mpi_grow( X, n ) );		//扩展n位
        MBEDTLS_MPI_CHK( mbedtls_mpi_lset( X, 0 ) );		//初始化每一位为0

        for( i = slen, j = 0; i > 0; i--, j++ )
        {
            if( i == 1 && s[i - 1] == '-' ) 				//判断字符串首位
            {
                X->s = -1;
                break;
            }

            MBEDTLS_MPI_CHK( mpi_get_digit( &d, radix, s[i - 1] ) );
            X->p[j / ( 2 * ciL )] |= d << ( ( j % ( 2 * ciL ) ) << 2 );
        }
    }
```
`i`初始为slen，也就是字符串最右边（数值上的低位），`j`初始为大数的最低位，mpi_get_digit将`i-1`位的char类型转为digit类型，这里ciL=8，所以`j / ( 2 * ciL )`意味着每16个字符为一组记为大数的1位。`d << ( ( j % ( 2 * ciL ) ) << 2 )`则表示这组16个字符每一位的数值，拆解来开，譬如字符串00001234，

```bash
i=7，d=4，j=0，左移0位
i=6，d=3，j=1，左移4位，乘以16
i=5，d=2，j=2，左移8位，乘以256
i=4，d=1，j=3，左移12位，乘以4096
```
以此类推求和。这里采用或运算而非加法。
### 输出字符串

```c
int mbedtls_mpi_write_string( const mbedtls_mpi *X, int radix,
                              char *buf, size_t buflen, size_t *olen )
```
过程与读取相反，作者写的非常精简：

```c
    if( X->s == -1 )
    {
        *p++ = '-';
        buflen--;
    }
    if( radix == 16 )
    {
        int c;
        size_t i, j, k;

        for( i = X->n, k = 0; i > 0; i-- )
        {
            for( j = ciL; j > 0; j-- )
            {
                c = ( X->p[i - 1] >> ( ( j - 1 ) << 3) ) & 0xFF;

                if( c == 0 && k == 0 && ( i + j ) != 2 )
                    continue;

                *(p++) = "0123456789ABCDEF" [c / 16];
                *(p++) = "0123456789ABCDEF" [c % 16];
                k = 1;
            }
        }
    }
```

除此之外还有操作文件的接口：

```c
int mbedtls_mpi_read_file( mbedtls_mpi *X, int radix, FILE *fin )
int mbedtls_mpi_write_file( const char *p, const mbedtls_mpi *X, int radix, FILE *fout )
```
### 数值比较
mbedtls提供了两个大数比较的接口，分别是原值和绝对值：
```c
int mbedtls_mpi_cmp_abs( const mbedtls_mpi *X, const mbedtls_mpi *Y )
int mbedtls_mpi_cmp_mpi( const mbedtls_mpi *X, const mbedtls_mpi *Y )
```
由于大数结构体存放了数位n，因此首先比较两者n的大小，对绝对值的情况，n越大，值越大；如果n相同，则从高位向后循环。
### 加减计算
大数加减提供四个主要函数：

```c
/*
 * Unsigned addition: X = |A| + |B|  (HAC 14.7)
 */
int mbedtls_mpi_add_abs( mbedtls_mpi *X, const mbedtls_mpi *A, const mbedtls_mpi *B )
/*
 * Unsigned subtraction: X = |A| - |B|  (HAC 14.9, 14.10)
 */
int mbedtls_mpi_sub_abs( mbedtls_mpi *X, const mbedtls_mpi *A, const mbedtls_mpi *B )
/*
 * Signed addition: X = A + B
 */
int mbedtls_mpi_add_mpi( mbedtls_mpi *X, const mbedtls_mpi *A, const mbedtls_mpi *B )
/*
 * Signed subtraction: X = A - B
 */
int mbedtls_mpi_sub_mpi( mbedtls_mpi *X, const mbedtls_mpi *A, const mbedtls_mpi *B )
```
首先是绝对值相加，然后是绝对值减法，有符号数加减法则可以是前两者的组合，例如正数加负数其实就是减法，负数加负数（或者负数减正数）则是绝对值加法取反。例如X=A+B的计算，
首先判断A和B的符号位乘积，如果为负数，
则比较A和B的绝对值，
* 绝对值A>B，则为绝对值A-B，符号位与A一致
* 绝对值A<B，则为绝对值B-A，符号位与A相反

如果为正数，则为绝对值A+B，符号位与A一致

### 乘法运算
乘法提供两个接口

```c
/*
 * Baseline multiplication: X = A * B  (HAC 14.12)
 */
int mbedtls_mpi_mul_mpi( mbedtls_mpi *X, const mbedtls_mpi *A, const mbedtls_mpi *B )
/*
 * Baseline multiplication: X = A * b
 */
int mbedtls_mpi_mul_int( mbedtls_mpi *X, const mbedtls_mpi *A, mbedtls_mpi_uint b )
```
### 大数除法
```c
/*
 * Division by mbedtls_mpi: A = Q * B + R  (HAC 14.20)
 */
int mbedtls_mpi_div_mpi( mbedtls_mpi *Q, mbedtls_mpi *R, const mbedtls_mpi *A,
                         const mbedtls_mpi *B )
```
这里Q=A/B，R=A mod B
### 取模运算
其实就是上面的R=0
```c
/*
 * Modulo: R = A mod B
 */
int mbedtls_mpi_mod_mpi( mbedtls_mpi *R, const mbedtls_mpi *A, const mbedtls_mpi *B )
```
### 指数运算
因为结果可能非常大，所以对结果取模N，即
$$
X = A^{E} mod N
$$

```c
/*
 * Sliding-window exponentiation: X = A^E mod N  (HAC 14.85)
 */
int mbedtls_mpi_exp_mod( mbedtls_mpi *X, const mbedtls_mpi *A,
                         const mbedtls_mpi *E, const mbedtls_mpi *N,
                         mbedtls_mpi *_RR )
```
### 求取最大公约数

```c
/*
 * Greatest common divisor: G = gcd(A, B)  (HAC 14.54)
 */
int mbedtls_mpi_gcd( mbedtls_mpi *G, const mbedtls_mpi *A, const mbedtls_mpi *B )
```

### 模逆运算
这里是求乘法逆元，即找到一个数X使得A和X的积关于模N与1同余，或者说
$$
A*X mod N = 1, X=A^{-1} mod N
$$
```c
/*
 * Modular inverse: X = A^-1 mod N  (HAC 14.61 / 14.64)
 */
int mbedtls_mpi_inv_mod( mbedtls_mpi *X, const mbedtls_mpi *A, const mbedtls_mpi *N )
```
