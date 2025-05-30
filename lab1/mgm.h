#ifndef MGM_H
#define MGM_H

#include <stdint.h>
#include <stddef.h>

void mgm_encrypt(
    uint8_t* ciphertext,
    uint8_t* tag,
    const uint8_t* plaintext,
    size_t len,
    const uint8_t* nonce,
    const uint32_t* rk
);

void mgm_decrypt(
    uint8_t* plaintext,
    const uint8_t* ciphertext,
    size_t len,
    const uint8_t* nonce,
    const uint32_t* rk
);

#endif
