#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "magma.h"

void xor_block(uint8_t* dst, const uint8_t* src, size_t len) {
    for (size_t i = 0; i < len; ++i)
        dst[i] ^= src[i];
}

void inc_counter(uint8_t* counter) {
    for (int i = BLOCK_SIZE - 1; i >= 0; --i)
        if (++counter[i]) break;
}

void mgm_encrypt(
    uint8_t* ciphertext,
    uint8_t* tag,
    const uint8_t* plaintext,
    size_t len,
    const uint8_t* nonce,
    const uint32_t* rk
) {
    uint8_t counter[BLOCK_SIZE];
    uint8_t gamma[BLOCK_SIZE];
    uint8_t mac[BLOCK_SIZE] = {0};
    memcpy(counter, nonce, BLOCK_SIZE);

    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        memcpy(gamma, counter, BLOCK_SIZE);
        encrypt_block(gamma, rk);

        size_t block_len = (len - i < BLOCK_SIZE) ? (len - i) : BLOCK_SIZE;
        for (size_t j = 0; j < block_len; ++j)
            ciphertext[i + j] = plaintext[i + j] ^ gamma[j];

        xor_block(mac, &ciphertext[i], block_len); // имитовставка
        inc_counter(counter);
    }

    memcpy(tag, mac, BLOCK_SIZE);
}

void mgm_decrypt(
    uint8_t* plaintext,
    const uint8_t* ciphertext,
    size_t len,
    const uint8_t* nonce,
    const uint32_t* rk
) {
    uint8_t counter[BLOCK_SIZE];
    uint8_t gamma[BLOCK_SIZE];
    memcpy(counter, nonce, BLOCK_SIZE);

    for (size_t i = 0; i < len; i += BLOCK_SIZE) {
        memcpy(gamma, counter, BLOCK_SIZE);
        encrypt_block(gamma, rk);

        size_t block_len = (len - i < BLOCK_SIZE) ? (len - i) : BLOCK_SIZE;
        for (size_t j = 0; j < block_len; ++j)
            plaintext[i + j] = ciphertext[i + j] ^ gamma[j];

        inc_counter(counter);
    }
}
