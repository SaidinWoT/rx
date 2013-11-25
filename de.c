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
        l = fgetc(in);
        if(l == EOF) {
            decrypt(buf, argv[1]);
            l = buf[--n];
            while(n >= 0 && buf[n] == (char)l)
                --n;
            if(++n != 0) {
                fwrite(buf, 1, n, out);
            }
            break;
        }
        ungetc(l, in);

        decrypt(buf, argv[1]);
        fwrite(buf, 1, MAXBYTES, out);
    }

    fclose(in);
    fclose(out);

    return 0;
}
