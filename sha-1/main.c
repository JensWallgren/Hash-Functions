
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


void print_endian_rev(uint32_t n) {
    uint32_t num = 0;
    for (int i = 0; i < 4; ++i) {
        num += ((n >> ((4-1-i) * 8)) & 0xff) << ((4-i-1)*8);
        //printf("%d : %x\n", i, ((n >> ((4-1-i) * 8)) & 0xff) << ((4-1-i)*8));
    }

    printf("%lu", num);
}

uint32_t left_rotate(uint32_t x, int n) {
    return (x << n) | (x >> (32-n));
}

uint32_t add(uint32_t a, uint32_t b) {
    return ((uint64_t)a + (uint64_t)b) % ((uint64_t)2 >> 32);
}

uint32_t f(int t, uint32_t b, uint32_t c, uint32_t d) {
    if (t >= 0 && t < 20)
        return (b & c) | (~b & d);
    else if ((t >= 20 && t < 40) || (t >= 60 && t < 80))
        return b ^ c ^ d;
    else if (t >= 40 && t < 60)
        return (b & c) | (b & d) | (c & d);
    else
        assert(0);
}

uint32_t K(int t) {
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

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("\nError: Wrong number of arguments."
               "\nUsage: [program_name].exe [input_string]\n\n");
        return 1;
    }
    
    size_t len = strlen(argv[1]);
    int pad = (56 - len) % 64;
    pad = pad ? pad : 56;

    uint8_t *input = malloc(80);
    strcpy(input, argv[1]);

    for (int i = len; i < len + pad; ++i)
        input[i] = i == len ? FIRST_PAD : PAD;

    uint32_t *ptr = (uint32_t *)(input + len + pad);
    ptr[0] = (0);
    ptr[1] = endian_rev_32(len * 8);
    /*
    for (int i = 0; i < 8; ++i) {
        ptr[i] = (len * 8) << (8-i)*8;
    }

    *((uint32_t *)(input+len+pad + 0)) = (len * 8) << 32;
    *((uint32_t *)(input+len+pad + 4)) = (len * 8);
    */

    uint32_t H[] = {
        0x67452301,
        0xefcdab89,
        0x98badcfe,
        0x10325476,
        0xc3d2e1f0
    };
    uint32_t W[80] = {0};

    for (int i = 0; i < len; i += 16) {

        for (int i = 0; i < 16; ++i) {
            uint32_t *p = (uint32_t *)input;
            W[i] = endian_rev_32(p[i]);
        }
        //memcpy(W, input, 16 * sizeof(uint32_t));

        uint32_t * w = W;
        for (int i = 0; i < 16; ++i) {
            print_endian_rev(W[i]);
            printf("\n");
        }

        for (int t = 16; t < 80; ++t) {
            W[t] = left_rotate(W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16], 1);
        }

        uint32_t a = H[0];
        uint32_t b = H[1];
        uint32_t c = H[2];
        uint32_t d = H[3];
        uint32_t e = H[4];

        int count = 0;
        for (int t = 0; t < 80; ++t) {
            uint32_t temp = endian_rev_32(left_rotate(a, 5)) + endian_rev_32(f(t, b, c, d)) + endian_rev_32(e) + endian_rev_32(W[t]) + K(t);
            temp = endian_rev_32(temp);

            e = d;
            d = c;
            c = left_rotate(b, 30);
            b = a;
            a = temp;

            H[0] = endian_rev_32(endian_rev_32(H[0]) + endian_rev_32(a));
            H[1] = endian_rev_32(endian_rev_32(H[1]) + endian_rev_32(b));
            H[2] = endian_rev_32(endian_rev_32(H[2]) + endian_rev_32(c));
            H[3] = endian_rev_32(endian_rev_32(H[3]) + endian_rev_32(d));
            H[4] = endian_rev_32(endian_rev_32(H[4]) + endian_rev_32(e));
            printf("[%d]", t);
            //print_endian_rev(W[t]);
            //print_bytes(W[t]);
            printf("%lu", W[t]);
            printf("\n");
        }
    }


    /*
    uint8_t hash[16];
    for (int i = 0; i < 4; ++i) { hash[0*4+i] = (a >> (8*i)) & 0xff; }
    for (int i = 0; i < 4; ++i) { hash[1*4+i] = (b >> (8*i))p& 0xff; }
    for (int i = 0; i < 4; ++i) { hash[2*4+i] = (c >> (8*i)) & 0xff; }
    for (int i = 0; i < 4; ++i) { hash[3*4+i] = (d >> (8*i)) & 0xff; }

    printf("\n\nHASH: ");
    for (int i = 0; i < 16; ++i) { printf("%x", hash[i]); }
    printf("\n\n");
    */

    return 0;
}

