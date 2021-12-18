#include <stdio.h>
#include "sha256.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("\nError: Wrong number of arguments.\nUsage: [program_name].exe [input_string]\n\n");
        return 1;
    }
    
    char *hash = produce_hash(argv[1], 0);
    printf("Hash: %s\n", hash);
    return 0;
}


