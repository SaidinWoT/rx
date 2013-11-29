#ifndef __RXE_H
#define __RXE_H

#define MAXBYTES 8
#define MAXBITS MAXBYTES*8

void encrypt(char* buf, char *key);
void decrypt(char* buf, char *key);
char en(char *buf, char *key, int len, int nxtChr);
char de(char *buf, char *key, int len, int nxtChr);

#endif /* !__RXE_H */
