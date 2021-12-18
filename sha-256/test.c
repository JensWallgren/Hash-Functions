#include <stdio.h>
#include <string.h>

#include "sha256.h"



int test(int test_number, char *input, char *answer) {
    if (strcmp(produce_hash(input, 0), answer) == 0)
        printf("Test %d: OK\n", test_number);
    else
        printf("Test %d: FAILED\n", test_number);

}

int main(int argc, char **argv) {
    test(0, "abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
    test(1, "", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
    test(2, "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
         "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");
    return 0;
}


