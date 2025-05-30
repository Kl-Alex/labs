#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include "kdf_tree.h"

void print_mem_usage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    printf("Память: %ld KB\n", usage.ru_maxrss);
}

int main() {
    const size_t iterations = 1000000;
    const uint8_t master_key[32] = {0};
    const char label[] = "benchmark";
    const char seed[] = "12345678";
    const uint32_t R = 1;
    const uint32_t L = 32;
    uint8_t out[64];

    printf("Начало тестирования производительности генерации %zu ключей...\n", iterations);
    print_mem_usage();

    clock_t start = clock();

    for (size_t i = 0; i < iterations; ++i) {
        KDF_TREE_GOSTR3411_2012_256(
            master_key, sizeof(master_key),
            (const uint8_t*)label, strlen(label),
            (const uint8_t*)seed, strlen(seed),
            R, L, out);
    }

    clock_t end = clock();

    print_mem_usage();
    double total = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Общее время: %.4f сек\n", total);
    printf("Скорость: %.2f ключей/сек\n", iterations / total);

    return 0;
}
