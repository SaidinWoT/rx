#include "rxe.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    FILE *in, *out;
    char n, l;
    char *buf = calloc(MAXBYTES, sizeof(char));
    in = fopen(argv[2], "r");
    if(argc > 3) {
        out = fopen(argv[3], "w");
    } else {
        out = stdout;
    }
    
    while(!feof(in) && !ferror(in)) {
        n = fread(buf, 1, MAXBYTES, in);
        if(feof(in)) {
            l = (char)((short)(5 * buf[n-1] + 1) & 0xFF);
            if(n == MAXBYTES) {
                encrypt(buf, argv[1]);
                fwrite(buf, 1, MAXBYTES, out);
                n = 0;
            }
            while(n < MAXBYTES) {
                buf[n++] = l;
            }
        }

        encrypt(buf, argv[1]);
        fwrite(buf, 1, MAXBYTES, out);
    }

    fclose(in);
    fclose(out);

    return 0;
}
