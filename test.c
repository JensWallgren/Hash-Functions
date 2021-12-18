#include <stdio.h>
#include <string.h>

//#include "md5/lib.h"
//#include "sha1/lib.h"
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
    test(sha256, "sha-256, 1: ", "abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
    test(sha256, "sha-256, 2: ", "", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    test(sha256, "sha-256, 3: ", "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
         "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");
    test(sha256, "sha-256, 4: ", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
         "8c48280d57fb88f161adf34d9f597d93ca32b7edfcd23b2afe64c3789b3f7855");
    return 0;
}


