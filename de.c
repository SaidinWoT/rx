#include "rxe.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    FILE *in, *out;
    char n;
    char *buf = calloc(MAXBYTES, sizeof(char));
    int l;
    in = fopen(argv[2], "r");
    if(argc > 3) {
        out = fopen(argv[3], "w");
    } else {
        out = stdout;
    }
    
    while(!feof(in) && !ferror(in)) {
        n = fread(buf, 1, MAXBYTES, in);

        decrypt(buf, argv[1]);

        l = fgetc(in);
        if(l == EOF) {
            l = buf[--n];
            while(n >= 0 && buf[n] == (char)l)
                --n;
            ++n;
        }
        ungetc(l, in);

        fwrite(buf, 1, n, out);
    }

    free(buf);
    fclose(in);
    fclose(out);

    return 0;
}
