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

static void(*fns[8])(char *) = {gll, grl, glr, grr, sll, srl, slr, srr};

inline void mixin(char *buf, char keybyte) {
    unsigned char j;
    for(j = 0; j < MAXBYTES; ++j) {
        buf[j] ^= keybyte*((keybyte >> j) & 1);
    }
}

void encrypt(char *buf, char *key) {
    int i, len;
    unsigned char j, k;
    len = strlen(key);
    for(i = 0; i < len; ++i) {
        k = 0;
        mixin(buf, key[i]);
        for(j = 0; j < MAXBYTES; ++j) {
            k += (~key[i] >> j) & 1;
            k %= 8;
            fns[k](buf);
            k += (key[i] >> j) & 1;
        }
        mixin(buf, key[i]);
    }
}

void decrypt(char *buf, char *key) {
    int i;
    unsigned char j, k;
    for(i = strlen(key) - 1; i >= 0; --i) {
        k = 4;
        mixin(buf, key[i]);
        for(j = 0; j < MAXBYTES; ++j) {
            k -= (key[i] >> (7-j)) & 1;
            k %= 8;
            fns[k](buf);
            k -= (~key[i] >> (7-j)) & 1;
        }
        mixin(buf, key[i]);
    }
}

char en(char *buf, char *key, int len, int atEOF) {
    int n = (len == MAXBYTES ? 2*MAXBYTES : MAXBYTES);
    char l;
    if(atEOF) {
        l = (char)((short)(5 * buf[len-1] + 1) & 0xFF);
        while(len < n) {
            buf[len++] = l;
        }
        encrypt(buf + MAXBYTES, key);
    }
    encrypt(buf, key);
    return len;
}

char de(char *buf, char *key, int len, int atEOF) {
    char l;
    decrypt(buf, key);
    if(atEOF) {
        l = buf[--len];
        while(len-- && buf[len] == (char)l)
            ;
        ++len;
    }
    return len;
}
