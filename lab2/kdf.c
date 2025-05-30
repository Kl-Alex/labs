#include "kdf.h"
#include <openssl/evp.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

static void int_to_be(uint32_t val, uint8_t out[4]) {
    out[0] = (val >> 24) & 0xFF;
    out[1] = (val >> 16) & 0xFF;
    out[2] = (val >> 8) & 0xFF;
    out[3] = val & 0xFF;
}

void kdf_gostr3411_2012_256(const uint8_t *master, size_t master_len,
                             const uint8_t *label, size_t label_len,
                             const uint8_t *seed, size_t seed_len,
                             uint8_t *output, size_t output_len) {
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_get_digestbyname("md_gost12_256");

    uint32_t counter = 1;
    size_t out_offset = 0;
    while (out_offset < output_len) {
        uint8_t counter_be[4];
        int_to_be(counter, counter_be);

        EVP_DigestInit_ex(mdctx, md, NULL);
        EVP_DigestUpdate(mdctx, counter_be, 4);
        EVP_DigestUpdate(mdctx, label, label_len);
        EVP_DigestUpdate(mdctx, seed, seed_len);
        EVP_DigestUpdate(mdctx, master, master_len);

        uint8_t hash[32];
        unsigned int hash_len;
        EVP_DigestFinal_ex(mdctx, hash, &hash_len);

        size_t copy_len = (output_len - out_offset > 32) ? 32 : (output_len - out_offset);
        memcpy(output + out_offset, hash, copy_len);
        out_offset += copy_len;
        counter++;
    }

    EVP_MD_CTX_free(mdctx);
}
