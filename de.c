#include "rxe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    FILE *in, *out;
    char *buf = calloc(8, sizeof(char));
    char *fer = calloc(8, sizeof(char));
    in = fopen(argv[2], "r");
    if(argc > 3) {
        out = fopen(argv[3], "w");
    } else {
        out = stdout;
    }
    
    while(!feof(in) && !ferror(in)) {
        if(!fread(buf, 1, 8, in)) {
            break;
        }

        decrypt(buf, fer, argv[1]);
        fwrite(fer, 1, 8, out);
    }

    fclose(in);
    fclose(out);

    return 0;
}
