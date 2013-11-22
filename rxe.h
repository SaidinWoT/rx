#ifndef __RXE_H
#define __RXE_H

#define MAXBYTES 8
#define MAXBITS MAXBYTES*8

void encrypt(char* in, char *out, char *key);
void decrypt(char* in, char *out, char *key);

#endif /* !__RXE_H */
