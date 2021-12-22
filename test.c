#include <stdio.h>
#include <string.h>

#include "md5/md5.h"
#include "sha1/sha1.h"
#include "sha256/sha256.h"


typedef char *(*hash_function)(char *input_string, int print_debug_info);

int test(hash_function fn, char *test_name, char *input, char *answer) {
    char *hash = fn(input, 0);
    if (strcmp(hash, answer) == 0) {
        printf("Test %s: OK\n", test_name);
    } else {
        printf("Test %s: FAILED\n", test_name);
    }
    free(hash);
}

int main(int argc, char **argv) {

    // MD5 tests
    test(md5, "md5, 1: ", "abc", "900150983cd24fb0d6963f7d28e17f72");
    test(md5, "md5, 2: ", "", "d41d8cd98f00b204e9800998ecf8427e");
    test(md5, "md5, 3: ", "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
         "8215ef0796a20bcaaae116d3876c664a");
    test(md5, "md5, 4: ", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
         "986e6938ed767a8ae9530eef54bfe5f1");


    // SHA1 tests
    test(sha1, "sha-1, 1: ", "abc", "a9993e364706816aba3e25717850c26c9cd0d89d");
    test(sha1, "sha-1, 2: ", "", "da39a3ee5e6b4b0d3255bfef95601890afd80709");
    test(sha1, "sha-1, 3: ", "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
         "84983e441c3bd26ebaae4aa1f95129e5e54670f1");
    test(sha1, "sha-1, 4: ", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
         "5832a02d8a00c665b4ec18f9dfcbe54979caa05b");

    // SHA256 tests
    test(sha256, "sha-256, 1: ", "abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
    test(sha256, "sha-256, 2: ", "", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    test(sha256, "sha-256, 3: ", "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
         "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");
    test(sha256, "sha-256, 4: ", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
         "8c48280d57fb88f161adf34d9f597d93ca32b7edfcd23b2afe64c3789b3f7855");
    return 0;
}


