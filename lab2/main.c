#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "kdf.h"

int main() {
    uint8_t master_key[32] = {0};
    uint8_t label[] = "Diversification";
    uint8_t seed[] = "StudentNameOrID";
    uint8_t derived_key[32];

    srand((unsigned)time(NULL));
    for (int i = 0; i < 32; i++) master_key[i] = rand() % 256;

    clock_t start = clock();
    for (int i = 0; i < 100000; i++) {
        kdf_gostr3411_2012_256(master_key, sizeof(master_key), label, sizeof(label)-1,
                               seed, sizeof(seed)-1, derived_key, sizeof(derived_key));
    }
    clock_t end = clock();

    printf("Пример полученного ключа: ");
    for (int i = 0; i < 32; i++) printf("%02X", derived_key[i]);
    printf("\n");

    printf("Время генерации 10^5 ключей: %.2f сек\n",
           (double)(end - start)/CLOCKS_PER_SEC);
    return 0;
}
