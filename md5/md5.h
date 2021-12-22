
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>


uint32_t md5_aux_f(uint32_t x, uint32_t y, uint32_t z) {
    return x & y | ~x & z;
}
uint32_t md5_aux_g(uint32_t x, uint32_t y, uint32_t z) {
    return x & z | y & ~z;
}
uint32_t md5_aux_h(uint32_t x, uint32_t y, uint32_t z) {
    return x ^ y ^ z;
}
uint32_t md5_aux_i(uint32_t x, uint32_t y, uint32_t z) {
    return y ^ (x | ~z);
}

uint64_t md5_sin[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,

    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x2441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,

    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x4881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,

    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};


uint64_t md5_left_rotate(uint32_t x, int n) {
    return (x << n) | (x >> (32-n));
}


void md5_round_1(uint32_t *a, uint32_t b, uint32_t c, uint32_t d,
             uint32_t x, int rot_amount, uint32_t sin_val)
{
    *a = b + md5_left_rotate(*a + md5_aux_f(b,c,d) + x + sin_val, rot_amount);
}

void md5_round_2(uint32_t *a, uint32_t b, uint32_t c, uint32_t d,
             uint32_t x, int rot_amount, uint32_t sin_val)
{
    *a = b + md5_left_rotate(*a + md5_aux_g(b,c,d) + x + sin_val, rot_amount);
}

void md5_round_3(uint32_t *a, uint32_t b, uint32_t c, uint32_t d,
             uint32_t x, int rot_amount, uint32_t sin_val)
{
    *a = b + md5_left_rotate(*a + md5_aux_h(b,c,d) + x + sin_val, rot_amount);
}

void md5_round_4(uint32_t *a, uint32_t b, uint32_t c, uint32_t d,
             uint32_t x, int rot_amount, uint32_t sin_val)
{
    *a = b + md5_left_rotate(*a + md5_aux_i(b,c,d) + x + sin_val, rot_amount);
}



char *md5(char *input_string, int print_debug) {
    int len = strlen(input_string);
    int block_count = len / 64 + 1;
    int next_block_boundary = block_count * 64;
    int pad = next_block_boundary - len - 8;

    if (pad <= 0) {
        block_count += 1;
        pad += 64;
    }
    pad -= 8;

    unsigned char *input = calloc(len + pad, sizeof(uint64_t));
    strcpy(input, input_string);
    input[len] = 0b10000000;

    //*((uint64_t *)(input+len+pad)) = (uint64_t)(len * 8);
    uint64_t *embedded_length = &input[block_count * 64 - 8];
    *embedded_length = len * 8;

    uint32_t a = 0x67452301;
    uint32_t b = 0xefcdab89;
    uint32_t c = 0x98badcfe;
    uint32_t d = 0x10325476;
    uint32_t *block = (uint32_t *)&input[0];


    for (int i = 0; i < block_count * 64; i += 64) {
        uint32_t *block = (uint32_t *)&input[i];
        uint32_t start_of_block_a = a;
        uint32_t start_of_block_b = b;
        uint32_t start_of_block_c = c;
        uint32_t start_of_block_d = d;

        if (print_debug) {
            printf("Current block:  ", i);
            for (int j = 0; j < 64; ++j) printf("%02x ", input[i*64 + j]);
            printf("\n");
        }

        md5_round_1(&a, b, c, d, block[ 0],  7, md5_sin[ 0]);
        md5_round_1(&d, a, b, c, block[ 1], 12, md5_sin[ 1]);
        md5_round_1(&c, d, a, b, block[ 2], 17, md5_sin[ 2]);
        md5_round_1(&b, c, d, a, block[ 3], 22, md5_sin[ 3]);
        md5_round_1(&a, b, c, d, block[ 4],  7, md5_sin[ 4]);
        md5_round_1(&d, a, b, c, block[ 5], 12, md5_sin[ 5]);
        md5_round_1(&c, d, a, b, block[ 6], 17, md5_sin[ 6]);
        md5_round_1(&b, c, d, a, block[ 7], 22, md5_sin[ 7]);
        md5_round_1(&a, b, c, d, block[ 8],  7, md5_sin[ 8]);
        md5_round_1(&d, a, b, c, block[ 9], 12, md5_sin[ 9]);
        md5_round_1(&c, d, a, b, block[10], 17, md5_sin[10]);
        md5_round_1(&b, c, d, a, block[11], 22, md5_sin[11]);
        md5_round_1(&a, b, c, d, block[12],  7, md5_sin[12]);
        md5_round_1(&d, a, b, c, block[13], 12, md5_sin[13]);
        md5_round_1(&c, d, a, b, block[14], 17, md5_sin[14]);
        md5_round_1(&b, c, d, a, block[15], 22, md5_sin[15]);

        md5_round_2(&a, b, c, d, block[ 1],  5, md5_sin[16]);
        md5_round_2(&d, a, b, c, block[ 6],  9, md5_sin[17]);
        md5_round_2(&c, d, a, b, block[11], 14, md5_sin[18]);
        md5_round_2(&b, c, d, a, block[ 0], 20, md5_sin[19]);
        md5_round_2(&a, b, c, d, block[ 5],  5, md5_sin[20]);
        md5_round_2(&d, a, b, c, block[10],  9, md5_sin[21]);
        md5_round_2(&c, d, a, b, block[15], 14, md5_sin[22]);
        md5_round_2(&b, c, d, a, block[ 4], 20, md5_sin[23]);
        md5_round_2(&a, b, c, d, block[ 9],  5, md5_sin[24]);
        md5_round_2(&d, a, b, c, block[14],  9, md5_sin[25]);
        md5_round_2(&c, d, a, b, block[ 3], 14, md5_sin[26]);
        md5_round_2(&b, c, d, a, block[ 8], 20, md5_sin[27]);
        md5_round_2(&a, b, c, d, block[13],  5, md5_sin[28]);
        md5_round_2(&d, a, b, c, block[ 2],  9, md5_sin[29]);
        md5_round_2(&c, d, a, b, block[ 7], 14, md5_sin[30]);
        md5_round_2(&b, c, d, a, block[12], 20, md5_sin[31]);

        md5_round_3(&a, b, c, d, block[ 5],  4, md5_sin[32]);
        md5_round_3(&d, a, b, c, block[ 8], 11, md5_sin[33]);
        md5_round_3(&c, d, a, b, block[11], 16, md5_sin[34]);
        md5_round_3(&b, c, d, a, block[14], 23, md5_sin[35]);
        md5_round_3(&a, b, c, d, block[ 1],  4, md5_sin[36]);
        md5_round_3(&d, a, b, c, block[ 4], 11, md5_sin[37]);
        md5_round_3(&c, d, a, b, block[ 7], 16, md5_sin[38]);
        md5_round_3(&b, c, d, a, block[10], 23, md5_sin[39]);
        md5_round_3(&a, b, c, d, block[13],  4, md5_sin[40]);
        md5_round_3(&d, a, b, c, block[ 0], 11, md5_sin[41]);
        md5_round_3(&c, d, a, b, block[ 3], 16, md5_sin[42]);
        md5_round_3(&b, c, d, a, block[ 6], 23, md5_sin[43]);
        md5_round_3(&a, b, c, d, block[ 9],  4, md5_sin[44]);
        md5_round_3(&d, a, b, c, block[12], 11, md5_sin[45]);
        md5_round_3(&c, d, a, b, block[15], 16, md5_sin[46]);
        md5_round_3(&b, c, d, a, block[ 2], 23, md5_sin[47]);

        md5_round_4(&a, b, c, d, block[ 0],  6, md5_sin[48]);
        md5_round_4(&d, a, b, c, block[ 7], 10, md5_sin[49]);
        md5_round_4(&c, d, a, b, block[14], 15, md5_sin[50]);
        md5_round_4(&b, c, d, a, block[ 5], 21, md5_sin[51]);
        md5_round_4(&a, b, c, d, block[12],  6, md5_sin[52]);
        md5_round_4(&d, a, b, c, block[ 3], 10, md5_sin[53]);
        md5_round_4(&c, d, a, b, block[10], 15, md5_sin[54]);
        md5_round_4(&b, c, d, a, block[ 1], 21, md5_sin[55]);
        md5_round_4(&a, b, c, d, block[ 8],  6, md5_sin[56]);
        md5_round_4(&d, a, b, c, block[15], 10, md5_sin[57]);
        md5_round_4(&c, d, a, b, block[ 6], 15, md5_sin[58]);
        md5_round_4(&b, c, d, a, block[13], 21, md5_sin[59]);
        md5_round_4(&a, b, c, d, block[ 4],  6, md5_sin[60]);
        md5_round_4(&d, a, b, c, block[11], 10, md5_sin[61]);
        md5_round_4(&c, d, a, b, block[ 2], 15, md5_sin[62]);
        md5_round_4(&b, c, d, a, block[ 9], 21, md5_sin[63]);

        a += start_of_block_a;
        b += start_of_block_b;
        c += start_of_block_c;
        d += start_of_block_d;
    }

    uint8_t hash[16];
    for (int i = 0; i < 4; ++i) {
        hash[0*4+i] = (a >> (8*i)) & 0xff;
        hash[1*4+i] = (b >> (8*i)) & 0xff;
        hash[2*4+i] = (c >> (8*i)) & 0xff;
        hash[3*4+i] = (d >> (8*i)) & 0xff;
    }

    int hash_length = sizeof(hash) / sizeof(hash[0]);
    char *ret = calloc(hash_length * 2 + 1, 1);
    for (int i = 0; i < hash_length; ++i) {
        sprintf(ret, "%s%02x", ret, hash[i]);
    }

    if (print_debug)
        printf("\n\n---- FINAL HASH ----\n   Final hash is: %s\n", ret);

    return ret;
}

