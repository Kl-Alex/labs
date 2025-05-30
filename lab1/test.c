#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "magma.h"
#include "utils.h"
#include "mgm.h"

int main() {
    size_t sizes[] = {1 << 20, 100 << 20, 1000 << 20};  // 1MB, 100MB, 1000MB
    uint8_t key[32];
    uint8_t nonce[8] = {0};

    srand((unsigned) time(NULL));
    random_bytes(key, sizeof(key));
    random_bytes(nonce, sizeof(nonce));

    uint32_t rk[32];
    expand_key(rk, key);

    for (int i = 0; i < 3; ++i) {
        size_t size = sizes[i];
        uint8_t* pt = malloc(size);
        uint8_t* ct = malloc(size);
        uint8_t* dt = malloc(size);
        uint8_t tag[8];

        random_bytes(pt, size);

        clock_t start = clock();
        mgm_encrypt(ct, tag, pt, size, nonce, rk);
        clock_t mid = clock();
        mgm_decrypt(dt, ct, size, nonce, rk);
        clock_t end = clock();

        printf("Файл %zu MB: шифрование %.2fs, расшифрование %.2fs\n",
               size >> 20,
               (double)(mid - start)/CLOCKS_PER_SEC,
               (double)(end - mid)/CLOCKS_PER_SEC);

        if (memcmp(pt, dt, size) != 0)
            printf("❌ Ошибка в расшифровке файла %zu MB\n", size >> 20);
        else
            printf("✅ Успешно\n");

        free(pt); free(ct); free(dt);
    }

    // Тест 10^6 блоков
    size_t blocks = 1000000;
    uint8_t block[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    uint8_t cipher[8], out[8];

    for (int freq = 10; freq <= 1000; freq *= 10) {
        printf("Смена ключа каждые %d блоков:\n", freq);
        clock_t t0 = clock();
        for (size_t i = 0; i < blocks; ++i) {
            if (i % freq == 0) {
                random_bytes(key, sizeof(key));
                expand_key(rk, key);
            }
            memcpy(cipher, block, 8);
            encrypt_block(cipher, rk);
            decrypt_block(cipher, rk);
        }
        clock_t t1 = clock();
        printf("⏱ Время: %.2fs\n", (double)(t1 - t0) / CLOCKS_PER_SEC);
    }

    return 0;
}
