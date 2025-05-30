#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BLOCK_SIZE 8       // 64 бита
#define KEY_SIZE 32        // 256 бит
#define ROUNDS 32

// S-блоки по ГОСТ Р 34.12-2015
static const uint8_t SBOX[8][16] = {
    {12, 4, 6, 2, 10, 5, 11, 9, 14, 8, 13, 7, 0, 3, 15, 1},
    {6, 8, 2, 3, 9, 10, 5, 12, 1, 14, 4, 7, 11, 13, 0, 15},
    {11, 3, 5, 8, 2, 15, 10, 13, 14, 1, 7, 4, 12, 9, 6, 0},
    {12, 8, 2, 1, 13, 4, 10, 0, 7, 6, 5, 9, 3, 15, 11, 14},
    {7, 15, 5, 10, 8, 1, 6, 13, 0, 9, 3, 14, 11, 4, 2, 12},
    {5, 13, 15, 6, 9, 2, 12, 10, 11, 7, 8, 1, 4, 3, 14, 0},
    {8, 14, 7, 1, 10, 3, 0, 9, 6, 15, 5, 13, 2, 12, 11, 4},
    {15, 5, 2, 10, 11, 3, 6, 12, 7, 0, 4, 14, 9, 13, 1, 8}
};

// Левый циклический сдвиг
uint32_t rol(uint32_t val, int bits) {
    return (val << bits) | (val >> (32 - bits));
}

// Раундовая функция F
uint32_t F(uint32_t x, uint32_t k) {
    uint32_t a = x + k;
    uint8_t bytes[4];
    for (int i = 0; i < 4; ++i)
        bytes[i] = (a >> (8 * (3 - i))) & 0xFF;

    for (int i = 0; i < 4; ++i) {
        bytes[i] = (SBOX[2 * i][bytes[i] >> 4] << 4) | SBOX[2 * i + 1][bytes[i] & 0x0F];
    }

    uint32_t out = 0;
    for (int i = 0; i < 4; ++i)
        out |= ((uint32_t)bytes[i]) << (8 * (3 - i));
    return rol(out, 11);
}

// Расширение ключа на 32 раунда
void expand_key(uint32_t* round_keys, const uint8_t* key) {
    for (int i = 0; i < 8; ++i) {
        round_keys[i] = ((uint32_t)key[4 * i] << 24) |
                        ((uint32_t)key[4 * i + 1] << 16) |
                        ((uint32_t)key[4 * i + 2] << 8) |
                        ((uint32_t)key[4 * i + 3]);
    }
    for (int i = 8; i < 32; ++i) {
        if (i < 24) round_keys[i] = round_keys[i % 8];
        else round_keys[i] = round_keys[31 - i];
    }
}

// Шифрование одного блока (8 байт)
void encrypt_block(uint8_t* block, const uint32_t* round_keys) {
    uint32_t n1 = ((uint32_t*)block)[0];
    uint32_t n2 = ((uint32_t*)block)[1];

    for (int i = 0; i < ROUNDS; ++i) {
        uint32_t tmp = n1;
        n1 = n2 ^ F(n1, round_keys[i]);
        n2 = tmp;
    }

    ((uint32_t*)block)[0] = n2;
    ((uint32_t*)block)[1] = n1;
}

// Дешифрование одного блока
void decrypt_block(uint8_t* block, const uint32_t* round_keys) {
    uint32_t n1 = ((uint32_t*)block)[0];
    uint32_t n2 = ((uint32_t*)block)[1];

    for (int i = 0; i < ROUNDS; ++i) {
        uint32_t tmp = n1;
        n1 = n2 ^ F(n1, round_keys[31 - i]);
        n2 = tmp;
    }

    ((uint32_t*)block)[0] = n2;
    ((uint32_t*)block)[1] = n1;
}
