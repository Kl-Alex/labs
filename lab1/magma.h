#ifndef MAGMA_H
#define MAGMA_H

#include <stdint.h>

#define BLOCK_SIZE 8
#define KEY_SIZE 32
#define ROUNDS 32

void expand_key(uint32_t* round_keys, const uint8_t* key);
void encrypt_block(uint8_t* block, const uint32_t* round_keys);
void decrypt_block(uint8_t* block, const uint32_t* round_keys);

#endif
