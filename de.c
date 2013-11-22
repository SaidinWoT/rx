#include "rxe.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    FILE *in, *out;
    char *buf = calloc(MAXBYTES, sizeof(char));
    in = fopen(argv[2], "r");
    if(argc > 3) {
        out = fopen(argv[3], "w");
    } else {
        out = stdout;
    }
    
    while(!feof(in) && !ferror(in)) {
        if(!fread(buf, 1, MAXBYTES, in)) {
            break;
        }

        decrypt(buf, argv[1]);
        fwrite(buf, 1, MAXBYTES, out);
    }

    fclose(in);
    fclose(out);

    return 0;
}
