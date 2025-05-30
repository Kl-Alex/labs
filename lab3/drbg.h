#ifndef DRBG_H
#define DRBG_H

#include <stdint.h>
#include <stddef.h>

#define DRBG_SEEDLEN 32
#define DRBG_OUTLEN  32

typedef struct {
    uint8_t K[DRBG_OUTLEN];
    uint8_t V[DRBG_OUTLEN];
    int reseed_counter;
} DRBG_CTX;

void drbg_init(DRBG_CTX *ctx, const uint8_t *entropy, size_t entropy_len);
void drbg_generate(DRBG_CTX *ctx, uint8_t *out, size_t outlen);
void drbg_reseed(DRBG_CTX *ctx, const uint8_t *entropy, size_t entropy_len);

#endif // DRBG_H
