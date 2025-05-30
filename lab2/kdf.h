#ifndef KDF_H
#define KDF_H

#include <stdint.h>
#include <stddef.h>

void kdf_gostr3411_2012_256(const uint8_t *master, size_t master_len,
                             const uint8_t *label, size_t label_len,
                             const uint8_t *seed, size_t seed_len,
                             uint8_t *output, size_t output_len);

#endif
