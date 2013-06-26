#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "darmbi.h"

int main()
{
#define BBL(len, ...) \
    assert(darmbi_bbl_length((uint32_t[]){__VA_ARGS__}, \
        sizeof((uint32_t[]){__VA_ARGS__})/sizeof(uint32_t)) == len)

    // and r0, r0, r0 ; and r0, r0, r0
    BBL(-1, 0xe0000000, 0xe0000000);

    // and r0, r0, r0 ; b <location>
    BBL(2, 0xe0000000, 0xea000000);

    // and r0, r0, r0 ; ldr r4, [r0, r3] ; add pc, r0, #8
    BBL(3, 0xe0000000, 0xe7904003, 0xe280f008);

    return 0;
}
