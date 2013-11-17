#include "rxe.h"
#include <string.h>

inline void srx(char *n, char l, void (*fn)(char *, char)) {
    if(!(l >>= 1)) {
        return;
    }
    (*fn)(n, l);
    srx(n, l, fn);
}

inline void grx(char *n, char l, void (*fn)(char *, char)) {
    if(l >= MAXBITS) {
        return;
    }
    (*fn)(n, l);
    grx(n, l << 1, fn);
}

inline void ll(char *n, char l) {
    unsigned char m;
    if(l < 8) {
        m = ((1 << l) - 1) << (8 - l);
        *n = (*n&~m)|((*n^((*n&(m>>l))<<l))&m);
    } else {
        l /= 8;
        for(m = 0; m < l; ++m) {
            n[m] ^= n[m+l];
        }
    }
}

inline void rl(char *n, char l) {
    unsigned char m;
    if(l < 8) {
        m = ((1 << l) - 1) << l;
        *n = (*n&~m)|((*n^((*n&(m>>l))<<l))&m);
    } else {
        l /= 8;
        for(m = 8 - 2*l; m < 8 - l; ++m) {
            n[m] ^= n[m+l];
        }
    }
}

inline void lr(char *n, char l) {
    unsigned char m;
    if(l < 8) {
        m = ((1 << l) - 1) << (8 - 2*l);
        *n = (*n&~m)|((*n^((*n&(m<<l))>>l))&m);
    } else {
        l /= 8;
        for(m = 0; m < l; ++m) {
            n[m+l] ^= n[m];
        }
    }
}

inline void rr(char *n, char l) {
    unsigned char m;
    if(l < 8) {
        m = (1 << l) - 1;
        *n = (*n&~m)|((*n^((*n&(m<<l))>>l))&m);
    } else {
        l /= 8;
        for(m = 8 - 2*l; m < 8 - l; ++m) {
            n[m+l] ^= n[m];
        }
    }
}

inline void gll(char *n) { grx(n, 1, ll); }
inline void grl(char *n) { grx(n, 1, rl); }
inline void glr(char *n) { grx(n, 1, lr); }
inline void grr(char *n) { grx(n, 1, rr); }
inline void sll(char *n) { srx(n, MAXBITS, ll); }
inline void srl(char *n) { srx(n, MAXBITS, rl); }
inline void slr(char *n) { srx(n, MAXBITS, lr); }
inline void srr(char *n) { srx(n, MAXBITS, rr); }

void encrypt(char *in, char *out, char *key) {
    static void (*fns[8])(char *) = {gll, grl, glr, grr, sll, srl, slr, srr};
    int i, j, k, len;
    len = strlen(key);
    for(i = 0; i < MAXBYTES; ++i) {
        out[i] = in[i];
    }
    for(i = 0; i < len; ++i) {
        k = 0;
        for(j = 0; j < MAXBYTES; ++j) {
            out[j] ^= ((key[i] >> j) & 1 ? key[i] : 0);
        }
        for(j = 0; j < MAXBYTES; ++j) {
            fns[k](out);
            k += (key[i] >> j) & 1;
        }
        for(j = 0; j < MAXBYTES; ++j) {
            out[j] ^= ((key[i] >> j) & 1 ? key[i] : 0);
        }
    }
}

void decrypt(char *in, char *out, char *key) {
    static void (*fns[8])(char *) = {sll, srl, slr, srr, gll, grl, glr, grr};
    int i, j, k;
    for(i = 0; i < MAXBYTES; ++i) {
        out[i] = in[i];
    }
    for(i = strlen(key) - 1; i >= 0; --i) {
        k = 0;
        for(j = 0; j < MAXBYTES; ++j) {
            k += (key[i] >> j) & 1;
            out[j] ^= ((key[i] >> j) & 1 ? key[i] : 0);
        }
        for(j = 0; j < MAXBYTES; ++j) {
            k -= (key[i] >> (7-j)) & 1;
            fns[k](out);
        }
        for(j = 0; j < MAXBYTES; ++j) {
            out[j] ^= ((key[i] >> j) & 1 ? key[i] : 0);
        }
    }
}
