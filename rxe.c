#include "rxe.h"
#include <stdio.h>
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
                slr(out);
            } else {
                srl(out);
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
                glr(out);
            } else {
                grl(out);
            }
        }
        for(j = 0; j < 8; ++j) {
            if((key[i] >> j) & 1) {
                out[j] ^= key[i];
            }
        }
    }
}
