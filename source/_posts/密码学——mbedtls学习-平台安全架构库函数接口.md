---
title: Mbedtls学习--平台安全架构库函数接口
date: 2021-06-10 13:50:03
tags:
    - mbedtls
    - 算法
categories: 
    - 密码学
---

# 平台安全架构
PSA，全称Platform Secure Architecture，平台安全架构，是ARM公司在物联网安全领域联合众多芯片厂商推出的一个重要的软件实现架构，主要内容包括：

<!-- more -->

 1. 威胁模型
 2.  安全分析
 3.  硬件和固件架构规范
 4.  开源固件参考实现
 5. 独立评估和认证计划--PSA Certified

TF-M是PSA的一种实现方式，主要基于ARM Cortex-M系列的芯片，除此之外还有TF-A等。[Mbedtls](https://tls.mbed.org/)也是ARM维护的基于ARM平台的开源嵌入式加密库，现已是[trust-frimware](https://www.trustedfirmware.org/)的一部分，除了基本的各类加解密算法及安全工具箱的实现，为方便嵌入式开发人员快速编写符合PSA规范的代码，mbedtls提供了各类安全工具的调用接口。源代码见mbdetls/library/psa_crypto.c文件。

# Mbedtls PSA库
## Key management
PSA支持对称和非对称密钥，实现基础是Mbedtls提供的各类密码箱工具。在导入key的时候需要注意key的type（类型）、usage（用途）和alg（算法）等属性，决定了密文或者签名的长度和解密的数据是否正确。Key在使用时，程序会检查key的policy的合法性，因此key的管理非常重要。主要的key管理函数如下：
* 设置和检查key的用途
  
```c
static void psa_set_key_usage_flags(psa_key_attributes_t *attributes,
                                    psa_key_usage_t usage_flags);
static psa_key_usage_t psa_get_key_usage_flags(
    const psa_key_attributes_t *attributes);
```
* 设置和检查key的算法

```c
static void psa_set_key_algorithm(psa_key_attributes_t *attributes,
                                  psa_algorithm_t alg);
static psa_algorithm_t psa_get_key_algorithm(
    const psa_key_attributes_t *attributes);
```
* 设置和检查key的类型

```c
static void psa_set_key_type(psa_key_attributes_t *attributes,
                             psa_key_type_t type);
static psa_key_type_t psa_get_key_type(const psa_key_attributes_t *attributes);
```
* 获取key的属性

```c
psa_status_t psa_get_key_attributes(mbedtls_svc_key_id_t key,
                                    psa_key_attributes_t *attributes);
```
* 删除key

```c
psa_status_t psa_destroy_key(mbedtls_svc_key_id_t key);
```
* 导入导出key，注意`psa_export_public_key`函数只能是非对称密钥类型。

```c
psa_status_t psa_import_key(const psa_key_attributes_t *attributes,
                            const uint8_t *data,
                            size_t data_length,
                            mbedtls_svc_key_id_t *key);
psa_status_t psa_export_key(mbedtls_svc_key_id_t key,
                            uint8_t *data,
                            size_t data_size,
                            size_t *data_length);
psa_status_t psa_export_public_key(mbedtls_svc_key_id_t key,
                                   uint8_t *data,
                                   size_t data_size,
                                   size_t *data_length);
```
导入key后，常用来使用的参数是`mbedtls_svc_key_id_t *key`，即`psa_key_id_t *key`，在TF-M中又被命名为`psa_key_handle_t *key`，这是一个重要的参数，而`psa_key_attributes_t *attributes`则是指key的属性，内容如下：

```c
typedef struct psa_client_key_attributes_s psa_key_attributes_t;

/* This is the client view of the `key_attributes` structure. Only
 * fields which need to be set by the PSA crypto client are present.
 * The PSA crypto service will maintain a different version of the
 * data structure internally. */
struct psa_client_key_attributes_s
{
    uint16_t type;
    uint16_t bits;
    uint32_t lifetime;
    psa_key_id_t id;
    uint32_t usage;
    uint32_t alg;
};
```
## Message digests
消息摘要，主要计算消息的单向散列函数值，计算hash的函数和Mbedtls类似，`psa_hash_operation_t *operation`需要先设置使用的算法，算法必须保证`PSA_ALG_IS_HASH(alg)`为真。
```c
psa_status_t psa_hash_setup(psa_hash_operation_t *operation,
                            psa_algorithm_t alg);
psa_status_t psa_hash_update(psa_hash_operation_t *operation,
                             const uint8_t *input,
                             size_t input_length);
psa_status_t psa_hash_finish(psa_hash_operation_t *operation,
                             uint8_t *hash,
                             size_t hash_size,
                             size_t *hash_length);
```
PSA提供了一个更简单的Hash计算接口，适合轻量化的计算，使用时需要注意确保hash的长度`hash_size`应当大于算法输出的长度（根据`PSA_HASH_LENGTH(alg)`计算）。

```c
psa_status_t psa_hash_compute(psa_algorithm_t alg,
                              const uint8_t *input,
                              size_t input_length,
                              uint8_t *hash,
                              size_t hash_size,
                              size_t *hash_length);
```
## MAC
消息认证码（Message authentication codes），用于认证消息，计算的输入为消息和消息发送者持有的密钥（对称密钥），计算和验证消息认证码，计算MAC的函数与Mbedtls类似，可以多次调用update函数计算长消息的MAC，`psa_mac_operation_t *operation`包含计算消息验证码用的算法和key的信息。
此处key的usage属性必须为`PSA_KEY_USAGE_SIGN_MESSAGE | PSA_KEY_USAGE_VERIFY_MESSAGE`，算法必须保证`PSA_ALG_IS_MAC(alg)`为真。
```c
psa_status_t psa_mac_sign_setup(psa_mac_operation_t *operation,
                                mbedtls_svc_key_id_t key,
                                psa_algorithm_t alg);
psa_status_t psa_mac_verify_setup(psa_mac_operation_t *operation,
                                  mbedtls_svc_key_id_t key,
                                  psa_algorithm_t alg);
psa_status_t psa_mac_update(psa_mac_operation_t *operation,
                            const uint8_t *input,
                            size_t input_length);
psa_status_t psa_mac_sign_finish(psa_mac_operation_t *operation,
                                 uint8_t *mac,
                                 size_t mac_size,
                                 size_t *mac_length);
psa_status_t psa_mac_verify_finish(psa_mac_operation_t *operation,
                                   const uint8_t *mac,
                                   size_t mac_length);
```
同样PSA提供了简短消息情况下的接口：

```c
psa_status_t psa_mac_compute(mbedtls_svc_key_id_t key,
                             psa_algorithm_t alg,
                             const uint8_t *input,
                             size_t input_length,
                             uint8_t *mac,
                             size_t mac_size,
                             size_t *mac_length);
psa_status_t psa_mac_verify(mbedtls_svc_key_id_t key,
                            psa_algorithm_t alg,
                            const uint8_t *input,
                            size_t input_length,
                            const uint8_t *mac,
                            size_t mac_length);
```
## Asymmetric cryptography 
非对称加解密和签名认证的接口。
### 非对称加解密
使用公钥加密，私钥解密，支持RSA、ECC等算法，key的usage为`PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT`。参数salt（盐）在算法不需要的时候填`NULL`。
```c
psa_status_t psa_asymmetric_encrypt(mbedtls_svc_key_id_t key,
                                    psa_algorithm_t alg,
                                    const uint8_t *input,
                                    size_t input_length,
                                    const uint8_t *salt,
                                    size_t salt_length,
                                    uint8_t *output,
                                    size_t output_size,
                                    size_t *output_length);
psa_status_t psa_asymmetric_decrypt(mbedtls_svc_key_id_t key,
                                    psa_algorithm_t alg,
                                    const uint8_t *input,
                                    size_t input_length,
                                    const uint8_t *salt,
                                    size_t salt_length,
                                    uint8_t *output,
                                    size_t output_size,
                                    size_t *output_length);
```

### 签名和验证
通常采用对消息的单向散列值使用私钥进行签名，而消息接收者验证签名的步骤如下：
* 对消息进行单向散列值计算
* 用公钥对签名进行解密
* 比对解密后的散列值和计算的散列值时否相同

上述过程是`psa_verify_hash`函数的执行过程。因此接口定义如下：

```c
psa_status_t psa_sign_hash(mbedtls_svc_key_id_t key,
                           psa_algorithm_t alg,
                           const uint8_t *hash,
                           size_t hash_length,
                           uint8_t *signature,
                           size_t signature_size,
                           size_t *signature_length);
psa_status_t psa_verify_hash(mbedtls_svc_key_id_t key,
                             psa_algorithm_t alg,
                             const uint8_t *hash,
                             size_t hash_length,
                             const uint8_t *signature,
                             size_t signature_length);
```
使用时key的usage为`PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH`，key的type必须是非对称密钥类型，算法需要和计算单向散列值的函数（例如`psa_hash_compute`）使用的算法保持一致。
## Symmetric cryptography
对称加解密的接口。key的usage为`PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT`,算法必须满足`PSA_ALG_IS_CIPHER(alg)`为真。

```c
psa_status_t psa_cipher_encrypt(mbedtls_svc_key_id_t key,
                                psa_algorithm_t alg,
                                const uint8_t *input,
                                size_t input_length,
                                uint8_t *output,
                                size_t output_size,
                                size_t *output_length);
psa_status_t psa_cipher_decrypt(mbedtls_svc_key_id_t key,
                                psa_algorithm_t alg,
                                const uint8_t *input,
                                size_t input_length,
                                uint8_t *output,
                                size_t output_size,
                                size_t *output_length);
```
上面的接口使用的是随机初始化向量IV，通过下列一组函数可以使用特定的初始化向量：

```c
psa_cipher_encrypt_setup
psa_cipher_decrypt_setup
psa_cipher_generate_iv
psa_cipher_set_iv
psa_cipher_update
psa_cipher_finish
```

## AEAD
Authenticated Encryption with Associated Data ([AEAD](https://blog.csdn.net/qq_35324057/article/details/106035773?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_title-0&spm=1001.2101.3001.4242))，对称密码和消息认证码，是一种同时具备保密性，完整性和可认证性的加密形式。key的usage为`PSA_KEY_USAGE_DECRYPT | PSA_KEY_USAGE_ENCRYPT`，算法需要保证`PSA_ALG_IS_AEAD(alg)`为真。nonce可以是随机数或初始化向量，additional_data用来认证，不放在消息中加密。
```c
psa_status_t psa_aead_encrypt(mbedtls_svc_key_id_t key,
                              psa_algorithm_t alg,
                              const uint8_t *nonce,
                              size_t nonce_length,
                              const uint8_t *additional_data,
                              size_t additional_data_length,
                              const uint8_t *plaintext,
                              size_t plaintext_length,
                              uint8_t *ciphertext,
                              size_t ciphertext_size,
                              size_t *ciphertext_length);
psa_status_t psa_aead_decrypt(mbedtls_svc_key_id_t key,
                              psa_algorithm_t alg,
                              const uint8_t *nonce,
                              size_t nonce_length,
                              const uint8_t *additional_data,
                              size_t additional_data_length,
                              const uint8_t *ciphertext,
                              size_t ciphertext_length,
                              uint8_t *plaintext,
                              size_t plaintext_size,
                              size_t *plaintext_length);
```
这里支持的算法包括：

```c
#define PSA_ALG_CCM                             ((psa_algorithm_t)0x05500100)
#define PSA_ALG_GCM                             ((psa_algorithm_t)0x05500200)
#define PSA_ALG_CHACHA20_POLY1305               ((psa_algorithm_t)0x05100500)
```
# 示例代码
## 签名认证
### 导入ECC非对称密钥

```c
#define ECC_P256_PUBLIC_KEY_SIZE PSA_KEY_EXPORT_ECC_PUBLIC_KEY_MAX_SIZE(256)
static uint8_t attestation_public_key[ECC_P256_PUBLIC_KEY_SIZE]; /* 65bytes */
static size_t attestation_public_key_len = 0;
static psa_key_handle_t public_key_handle = 0;

static void load_key_pair()
{
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_handle_t key_handle = 0;
    psa_ecc_family_t psa_curve;
    struct ecc_key_t attest_key = {0};
    uint8_t key_buf[3 * PSA_BITS_TO_BYTES(256)]; /* priv + x_coord + y_coord */
    enum tfm_plat_err_t plat_res;
    psa_key_usage_t usage = PSA_KEY_USAGE_SIGN_HASH | PSA_KEY_USAGE_VERIFY_HASH;

    tfm_plat_get_initial_attest_key(key_buf, sizeof(key_buf),
                                    &attest_key, &psa_curve);

    /* Setup the key policy for private key */
    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_algorithm(&key_attributes, PSA_ALG_ECDSA(PSA_ALG_SHA_256));
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_ECC_KEY_PAIR(psa_curve));

    /* Register private key to Crypto service */
    int32_t crypto_result = psa_import_key(&key_attributes,
                                           attest_key.priv_key,
                                           attest_key.priv_key_size,
                                           &key_handle);

    crypto_result = psa_export_public_key(key_handle, attestation_public_key,
                                          ECC_P256_PUBLIC_KEY_SIZE,
                                          &attestation_public_key_len);

    public_key_handle = key_handle;
}
```
### 计算签名

```c
static int spm_server_sign_certificate(psa_msg_t *msg)
{
    uint8_t server_msg[16] = {0};
    uint8_t server_msg_hash[32] = {0};
    size_t hash_length = 0;

    uint8_t signature[64] = {0};
    size_t signature_length = 0;
    int i = 0;

    /* read data */
    uint8_t msg_length = psa_read(msg->handle, 0,
                                  server_msg, sizeof(server_msg));

    /* calculate hash */
    int32_t crypto_result = psa_hash_compute(PSA_ALG_SHA_256,
                                             server_msg,
                                             msg_length,
                                             server_msg_hash,
                                             sizeof(server_msg_hash),
                                             &hash_length);

    /* calculate signature */
    crypto_result = psa_asymmetric_sign(public_key_handle,
                                        PSA_ALG_ECDSA(PSA_ALG_SHA_256),
                                        server_msg_hash,
                                        hash_length,
                                        signature,
                                        sizeof(signature),
                                        &signature_length);

    psa_write(msg->handle, 0, signature, signature_length);
    psa_write(msg->handle, 1, &crypto_result, sizeof(crypto_result));

    return crypto_result;
}
```
### 验证签名

```c
static int spm_client_verify_certificate(psa_msg_t *msg)
{
    uint8_t server_msg[16] = {0};
    uint8_t server_msg_hash[32] = {0};
    size_t hash_length = 0;
    int32_t verify_ret = 0;
    uint8_t signature[64] = {0};

    /* read data */
    uint8_t msg_length = psa_read(msg->handle, 0,
                                  server_msg, sizeof(server_msg));
    size_t signature_length = psa_read(msg->handle, 1,
                                       signature, sizeof(signature));

    /* calculate hash */
    int32_t crypto_result = psa_hash_compute(PSA_ALG_SHA_256,
                                             server_msg,
                                             msg_length,
                                             server_msg_hash,
                                             sizeof(server_msg_hash),
                                             &hash_length);

    /* verify signature */
    verify_ret = psa_asymmetric_verify(public_key_handle,
                                       PSA_ALG_ECDSA(PSA_ALG_SHA_256),
                                       server_msg_hash,
                                       hash_length,
                                       signature,
                                       signature_length);

    psa_write(msg->handle, 0, &verify_ret, sizeof(verify_ret));

    return 0;
}
```

## AEAD加密解密
### 导入对称密钥

```c
#define MAX_KEY_NUM 2
static psa_key_handle_t symmetric_key_handle[MAX_KEY_NUM] = {0};

static int32_t load_symmetric_key(int8_t key_index)
{
    if (key_index > MAX_KEY_NUM || key_index < 0)
    {
        return -1;
    }
    const uint8_t key_buf[] = "This is AES key";
    psa_key_attributes_t key_attributes = psa_key_attributes_init();
    psa_key_handle_t key_handle = 0;
    psa_key_usage_t usage = PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT;

    psa_set_key_usage_flags(&key_attributes, usage);
    psa_set_key_type(&key_attributes, PSA_KEY_TYPE_AES);
    psa_set_key_algorithm(&key_attributes, PSA_ALG_CCM);

    int32_t crypto_result = psa_import_key(&key_attributes, key_buf,
                                           sizeof(key_buf), &key_handle);

    symmetric_key_handle[key_index] = key_handle;
}
```
### 消息加密

```c
const size_t nonce_length = 12;
const uint8_t nonce[] = "01234567890";
const uint8_t associated_data[] = "This is associated data";

static int spm_client_encrypt_data(psa_msg_t *msg)
{
    int8_t key_index = -1;
    uint8_t plaintext[16] = {0};
    size_t plaintext_len = 0;
    uint8_t ciphertext[32] = {0};
    size_t ciphertext_len = 0;

    /* read data */
    int32_t result = psa_read(msg->handle, 0, &key_index, sizeof(key_index));
    plaintext_len = psa_read(msg->handle, 1, plaintext, sizeof(plaintext));

    /* encrypt data by key */
    result = psa_aead_encrypt(symmetric_key_handle[key_index],
                              PSA_ALG_CCM,
                              nonce,
                              nonce_length,
                              associated_data,
                              sizeof(associated_data),
                              plaintext,
                              plaintext_len,
                              ciphertext,
                              sizeof(ciphertext),
                              &ciphertext_len);

    psa_write(msg->handle, 0, ciphertext, ciphertext_len);
    psa_write(msg->handle, 1, &result, sizeof(result));

    return result;
}
```
### 消息解密

```c
const size_t nonce_length = 12;
const uint8_t nonce[] = "01234567890";
const uint8_t associated_data[] = "This is associated data";

static int spm_client_decrypt_data(psa_msg_t *msg)
{
    int8_t key_index = -1;
    uint8_t ciphertext[32] = {0};
    size_t ciphertext_len = 0;
    uint8_t decrypttext[16] = {0};
    size_t decrypttext_len = 0;

    /* read data */
    int32_t result = psa_read(msg->handle, 0, &key_index, sizeof(key_index));
    ciphertext_len = psa_read(msg->handle, 1, ciphertext, sizeof(ciphertext));

    /* decrypt data by key */
    result = psa_aead_decrypt(symmetric_key_handle[key_index],
                              PSA_ALG_CCM,
                              nonce,
                              nonce_length,
                              associated_data,
                              sizeof(associated_data),
                              ciphertext,
                              ciphertext_len,
                              decrypttext,
                              sizeof(decrypttext),
                              &decrypttext_len);

    psa_write(msg->handle, 0, decrypttext, decrypttext_len);
    psa_write(msg->handle, 1, &result, sizeof(result));

    return result;
}
```
