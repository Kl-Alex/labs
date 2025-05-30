// utils.c
#include "utils.h"
#include <stdlib.h>

void random_bytes(uint8_t* buf, size_t len) {
    for (size_t i = 0; i < len; ++i)
        buf[i] = rand() % 256;
}
