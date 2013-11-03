#include "rxe.h"
#include <stdio.h>
#include <string.h>

void rxen(char *n, char l, void (*fn)(char *, char)) {
    if(!(l >>= 1)) {
        return;
    }
    (*fn)(n, l);
    rxen(n, l, fn);
}

void rxde(char *n, char l, void (*fn)(char *, char)) {
    if(l >= MAXBITS) {
        return;
    }
    (*fn)(n, l);
    rxde(n, l << 1, fn);
}

inline void lrx(char *n, char l) {
    unsigned char m;
    if(l < 8) {
        m = ((1 << l) - 1) << (8 - 2*l);
        *n = (*n&~m)|((*n^((*n&(m<<l))>>l))&m);
    } else {
        for(m = 0; m < l/8; ++m) {
            n[m+l/8] ^= n[m];
        }
    }
}

inline void rrx(char *n, char l) {
    unsigned char m;
    if(l < 8) {
        m = ((1 << l) - 1) << l;
        *n = (*n&~m)|((*n^((*n&(m>>l))<<l))&m);
    } else {
        for(m = 0; m < l/8; ++m) {
            n[m] ^= n[m+l/8];
        }
    }
}

inline void lrxen(char *n) { rxen(n, MAXBITS, lrx); }
inline void rrxen(char *n) { rxen(n, MAXBITS, rrx); }
inline void lrxde(char *n) { rxde(n, 1, lrx); }
inline void rrxde(char *n) { rxde(n, 1, rrx); }

void encrypt(char *in, char *out, char *key) {
    int i, j, len;
    for(j = 0; j < 8; ++j) {
        out[j] = in[j];
    }
    len = strlen(key);
    for(i = 0; i < len; ++i) {
        for(j = 0; j < 8; ++j) {
            if((key[i] >> j) & 1) {
                out[j] ^= key[i];
            }
        }
        for(j = 0; j < 8; ++j) {
            if((key[i] >> j) & 1) {
                lrxen(out);
            } else {
                rrxen(out);
            }
        }
        for(j = 0; j < 8; ++j) {
            if((key[i] >> j) & 1) {
                out[j] ^= key[i];
            }
        }
    }
}

void decrypt(char *in, char *out, char *key) {
    int i, j;
    for(j = 0; j < 8; ++j) {
        out[j] = in[j];
    }
    for(i = strlen(key) - 1; i >= 0; --i) {
        for(j = 0; j < 8; ++j) {
            if((key[i] >> j) & 1) {
                out[j] ^= key[i];
            }
        }
        for(j = 0; j < 8; ++j) {
            if((key[i] >> (7-j)) & 1) {
                lrxde(out);
            } else {
                rrxde(out);
            }
        }
        for(j = 0; j < 8; ++j) {
            if((key[i] >> j) & 1) {
                out[j] ^= key[i];
            }
        }
    }
}
