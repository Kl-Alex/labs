#include "drbg.h"
#include <string.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>

static void hmac_sha256(const uint8_t *key, size_t key_len,
                        const uint8_t *data, size_t data_len,
                        uint8_t *out) {
    unsigned int len;
    HMAC(EVP_sha256(), key, key_len, data, data_len, out, &len);
}

void drbg_update(DRBG_CTX *ctx, const uint8_t *provided_data, size_t pd_len) {
    uint8_t temp[DRBG_OUTLEN];
    uint8_t sep[1] = {0x00};
    uint8_t input[DRBG_OUTLEN + 1 + pd_len];

    memcpy(input, ctx->V, DRBG_OUTLEN);
    memcpy(input + DRBG_OUTLEN, sep, 1);
    if (pd_len > 0) memcpy(input + DRBG_OUTLEN + 1, provided_data, pd_len);

    hmac_sha256(ctx->K, DRBG_OUTLEN, input, DRBG_OUTLEN + 1 + pd_len, temp);
    memcpy(ctx->K, temp, DRBG_OUTLEN);

    hmac_sha256(ctx->K, DRBG_OUTLEN, ctx->V, DRBG_OUTLEN, ctx->V);

}

void drbg_init(DRBG_CTX *ctx, const uint8_t *entropy, size_t entropy_len) {
    memset(ctx->K, 0x00, DRBG_OUTLEN);
    memset(ctx->V, 0x01, DRBG_OUTLEN);
    drbg_update(ctx, entropy, entropy_len);
    ctx->reseed_counter = 1;
}

void drbg_generate(DRBG_CTX *ctx, uint8_t *out, size_t outlen) {
    while (outlen > 0) {
        hmac_sha256(ctx->K, DRBG_OUTLEN, ctx->V, DRBG_OUTLEN, ctx->V);
        size_t len = (outlen > DRBG_OUTLEN) ? DRBG_OUTLEN : outlen;
        memcpy(out, ctx->V, len);
        out += len;
        outlen -= len;
    }
    drbg_update(ctx, NULL, 0);
    ctx->reseed_counter++;
}

void drbg_reseed(DRBG_CTX *ctx, const uint8_t *entropy, size_t entropy_len) {
    drbg_update(ctx, entropy, entropy_len);
    ctx->reseed_counter = 1;
}
