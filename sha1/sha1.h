#pragma once

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define FIRST_PAD 0b10000000
#define PAD       0b00000000

uint32_t sha1_endian_rev_32(uint32_t n) {
    uint32_t ret = 0;
    uint8_t *p = (uint8_t *)&ret;
    uint8_t *p_n = (uint8_t *)&n;
    for (int i = 0; i < 4; ++i)
        p[i] = p_n[4-1-i];
    return ret;
}


uint32_t sha1_left_rotate(uint32_t x, int n) {
    return (x << n) | (x >> (32-n));
}


uint32_t sha1_f(int t, uint32_t b, uint32_t c, uint32_t d) {
    if (t >= 0 && t < 20)
        return (b & c) | (~b & d);
    else if ((t >= 20 && t < 40) || (t >= 60 && t < 80))
        return b ^ c ^ d;
    else if (t >= 40 && t < 60)
        return (b & c) | (b & d) | (c & d);
    else
        assert(0);
}

uint32_t sha1_K(int t) {
    if (t >= 0 && t < 20)
        return 0x5a827999;
    else if (t >= 20 && t < 40)
        return 0x6ed9eba1;
    else if (t >= 40 && t < 60)
        return 0x8f1bbcdc;
    else if (t >= 60 && t < 80)
        return 0xca62c1d6;
    else
        assert(0);
}

char *sha1(char *input_string, int print_debug) {
    size_t len = strlen(input_string);
    size_t block_count = (len + 8) / 64 + 1;
    if (print_debug)
        printf("len: %d,  blocks: %d\n\n", len, block_count);

    int pad = (56 - len) % 64;
    pad = pad ? pad : 56;

    uint8_t *input = malloc(80 * block_count);
    strcpy(input, input_string);

    for (int i = len; i < len + pad; ++i)
        input[i] = i == len ? FIRST_PAD : PAD;

    uint32_t *ptr = (uint32_t *)(input + ((block_count - 1) * 64) + 56);
    ptr[0] = (0);
    ptr[1] = sha1_endian_rev_32(len * 8);

    uint32_t H[] = {
        0x67452301,
        0xefcdab89,
        0x98badcfe,
        0x10325476,
        0xc3d2e1f0
    };
    uint32_t W[80] = {0};


    for (int i = 0; i < block_count; ++i) {

        for (int j = 0; j < 16; ++j) {
            uint32_t *p = (uint32_t *)(input + i*64);
            W[j] = sha1_endian_rev_32(p[j]);
        }

        uint32_t * w = W;

        for (int t = 16; t < 80; ++t) {
            W[t] = sha1_left_rotate(W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16], 1);
        }

        uint32_t a = H[0];
        uint32_t b = H[1];
        uint32_t c = H[2];
        uint32_t d = H[3];
        uint32_t e = H[4];

        // Print padded and extended message
        if (print_debug) {
            for (int t = 0; t < 80; ++t) {
                printf("[%d]", t);
                printf("%lu", W[t]);
                printf("\n");
            }
        }

        int count = 0;
        for (int t = 0; t < 80; ++t) {
            uint32_t temp = sha1_left_rotate(a, 5) + sha1_f(t, b, c, d)
                + e + W[t] + sha1_K(t);

            e = d;
            d = c;
            c = sha1_left_rotate(b, 30);
            b = a;
            a = temp;

            if (print_debug) {
                printf("[t = %d] A=%lu, B=%lu, C=%lu, D=%lu, E=%lu\n",
                       t, a, b, c, d, e);
            }
        }

        H[0] = H[0] + a;
        H[1] = H[1] + b;
        H[2] = H[2] + c;
        H[3] = H[3] + d;
        H[4] = H[4] + e;

        if (print_debug) {
            printf("\nBlock %d processed -- "
                   "H[0]=%lu, H[1]=%lu, H[2]=%lu, H[3]=%lu, [4]=%lu\n",
                   i, H[0], H[1], H[2], H[3], H[4]);
        }

    }

    int H_length = sizeof(H) / sizeof(H[0]);
    char *hash = calloc(H_length * 8 + 1, 1);
    for (int i = 0; i < H_length; ++i) {
        sprintf(hash, "%s%08x", hash, H[i]);
    }

    if (print_debug)
        printf("\n\n---- FINAL HASH ----\n   Final hash is: %s\n", hash);
    return hash;
}

