#include "drbg.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void write_random_file(DRBG_CTX *ctx, const char *filename, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("fopen");
        return;
    }

    uint8_t buffer[4096];
    size_t written = 0;
    while (written < size) {
        size_t chunk = (size - written > sizeof(buffer)) ? sizeof(buffer) : (size - written);
        drbg_generate(ctx, buffer, chunk);
        fwrite(buffer, 1, chunk, f);
        written += chunk;
    }
    fclose(f);
}

int main() {
    DRBG_CTX ctx;
    uint8_t entropy[DRBG_SEEDLEN];

    srand((unsigned)time(NULL));
    for (int i = 0; i < DRBG_SEEDLEN; i++)
        entropy[i] = rand() & 0xFF;

    drbg_init(&ctx, entropy, DRBG_SEEDLEN);

    // Генерация 1МБ, 100МБ, 1000МБ
    struct {
        const char *name;
        size_t size;
    } files[] = {
        {"random_1MB.bin", 1 << 20},
        {"random_100MB.bin", 100 << 20},
        {"random_1000MB.bin", 1000 << 20}
    };

    for (int i = 0; i < 3; i++) {
        printf("Генерация файла %s (%zu байт)...\n", files[i].name, files[i].size);
        clock_t start = clock();
        write_random_file(&ctx, files[i].name, files[i].size);
        clock_t end = clock();
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Затрачено времени: %.2f сек\n", time_spent);
    }

    // Генерация ключей
    int counts[] = {1000, 10000};
    for (int j = 0; j < 2; j++) {
        printf("Генерация %d ключей длиной 32 байта...\n", counts[j]);
        clock_t start = clock();
        for (int i = 0; i < counts[j]; i++) {
            uint8_t key[32];
            drbg_generate(&ctx, key, 32);
        }
        clock_t end = clock();
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Время генерации %d ключей: %.4f сек\n", counts[j], time_spent);
    }

    return 0;
}
