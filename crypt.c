#include "rxe.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    FILE *in, *out;
    char (*crypt)(char *, char *, int, int) = en;
    char *buf;
    int c, n;
    while((c = getopt(argc, argv, "dh")) != -1) {
        switch(c) {
            case '?':
            case 'h':
                /* usage(); */
                return 0;
            case 'd':
                crypt = de;
                break;
        }
    }
    argv += optind;
    argc -= optind;
    buf = calloc(MAXBYTES, sizeof(char));
    in = fopen(argv[1], "r");
    out = argc > 2 ? fopen(argv[2], "w") : stdout;

    while(!feof(in) && !ferror(in)) {
        n = fread(buf, 1, MAXBYTES, in);
        c = fgetc(in);
        n = crypt(buf, argv[0], n, (c == EOF));
        fwrite(buf, 1, n, out);
        ungetc(c, in);
    }

    free(buf);
    fclose(in);
    fclose(out);

    return 0;
}
