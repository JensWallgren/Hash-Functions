#pragma once

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define FIRST_PAD 0b10000000
#define PAD       0b00000000

uint16_t endian_rev_16(uint16_t n) {
    uint16_t ret = 0;
    uint8_t *p = (uint8_t *)&ret;
    for (int i = 0; i < 2; ++i)
        p[i] = (n >> (i * 8)) & 0xff;
    return ret;
}
uint32_t endian_rev_32(uint32_t n) {
    uint32_t ret = 0;
    uint8_t *p = (uint8_t *)&ret;
    uint8_t *p_n = (uint8_t *)&n;
    for (int i = 0; i < 4; ++i)
        p[i] = p_n[4-1-i];
    return ret;
}
uint64_t endian_rev_64(uint64_t n) {
    uint64_t ret = 0;
    uint8_t *p = (uint8_t *)&ret;
    for (int i = 0; i < 8; ++i)
        p[i] = (n >> (i * 8)) & 0xff;
    return ret;
}

void print_bytes(uint32_t n) {
    uint8_t *p = (uint8_t *)&n;
    for (int i = 0; i < 4; ++i)
        printf("%2x ", p[i]);
}


uint32_t left_rotate(uint32_t x, int n) {
    return (x << n) | (x >> (32-n));
}

uint32_t right_rotate(uint32_t x, int n) {
    return (x >> n) | (x << (32-n));
}


uint32_t CH(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (~x & z);
}

uint32_t MAJ(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

uint32_t BSIG0(uint32_t x) {
    return right_rotate(x, 2) ^ right_rotate(x, 13) ^ right_rotate(x, 22);
}

uint32_t BSIG1(uint32_t x) {
    return right_rotate(x, 6) ^ right_rotate(x, 11) ^ right_rotate(x, 25);
}

uint32_t SSIG0(uint32_t x) {
    return right_rotate(x, 7) ^ right_rotate(x, 18) ^ (x >> 3);
}

uint32_t SSIG1(uint32_t x) {
    return right_rotate(x, 17) ^ right_rotate(x, 19) ^ (x >> 10);
}


void print_binary(uint8_t *ptr, int size) {
    for (int i = 0; i < size; ++i) {
        if (i % 8 == 0)
            printf("\n");

        for (int j = 0; j < 8; ++j) {
            if (ptr[i] >> (8-1-j) & 1)
                printf("1");
            else
                printf("0");
        }
        printf(" ");
    }
    printf("\n");
}



char *produce_hash(char *input_string, int print_debug_info) {
    size_t len = strlen(input_string);
    size_t block_count = (len + 8) / 64 + 1;
    if (print_debug_info)
        printf("len: %d,  blocks: %d\n\n", len, block_count);

    int pad = (56 - len) % 64;
    pad = pad ? pad : 56;

    uint8_t *input = calloc(64 * block_count, 1);
    strcpy(input, input_string);

    for (int i = len; i < len + pad; ++i)
        input[i] = i == len ? FIRST_PAD : PAD;

    uint32_t *ptr = (uint32_t *)(input + ((block_count - 1) * 64) + 56);
    ptr[0] = (0);
    ptr[1] = endian_rev_32(len * 8);

    if (print_debug_info)
        print_binary(input, 64);


    uint32_t H[] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    uint32_t K[] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };
    uint32_t W[64] = {0};


    for (int i = 0; i < block_count; ++i) {

        for (int j = 0; j < 16; ++j) {
            uint32_t *p = (uint32_t *)(input + i*64);
            W[j] = endian_rev_32(p[j]);
        }

        uint32_t * w = W;

        for (int t = 16; t < 64; ++t) {
            uint32_t sig1 = SSIG1(W[t-2]);
            uint32_t sig0 = SSIG0(W[t-15]);
            W[t] = SSIG1(W[t-2]) + W[t-7] + SSIG0(W[t-15]) + W[t-16];
        }
        if (print_debug_info)
            print_binary((uint8_t *)W, 64*3);

        uint32_t a = H[0];
        uint32_t b = H[1];
        uint32_t c = H[2];
        uint32_t d = H[3];
        uint32_t e = H[4];
        uint32_t f = H[5];
        uint32_t g = H[6];
        uint32_t h = H[7];

        // Print padded and extended message
        /*
        for (int t = 0; t < 64; ++t) {
            printf("[%d]", t);
            printf("%lu", W[t]);
            printf("\n");
        }
        */

        int count = 0;
        for (int t = 0; t < 64; ++t) {
            uint32_t T1 = h + BSIG1(e) + CH(e, f, g) + K[t] + W[t];
            uint32_t T2 = BSIG0(a) + MAJ(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
            if (print_debug_info) {
                printf("[t = %d] T1=%lu, T2=%lu, A=%lu, B=%lu, C=%lu, D=%lu, E=%lu\n",
                       t, T1, T2, a, b, c, d, e);
            }
        }

        H[0] = H[0] + a;
        H[1] = H[1] + b;
        H[2] = H[2] + c;
        H[3] = H[3] + d;
        H[4] = H[4] + e;
        H[5] = H[5] + f;
        H[6] = H[6] + g;
        H[7] = H[7] + h;

        if (print_debug_info) {
            printf("\nBlock %d processed -- H[0]=%lu, H[1]=%lu, H[2]=%lu, H[3]=%lu, [4]=%lu\n",
                   i, H[0], H[1], H[2], H[3], H[4]);
        }
    }

    // TODO: We're running into memory problems here. See Address Sanitizer.
    //char *hash = calloc(8*5+1, 1);
    char *hash = calloc(500, 1);
    for (int i = 0; i < 8; ++i) {
        sprintf(hash, "%s%08x", hash, H[i]);
    }

    if (print_debug_info)
        printf("\n\n---- FINAL HASH ----\n   Final hash is: %s\n", hash);

    return hash;
}


