#ifndef __RXE_H
#define __RXE_H

#define MAXBYTES 8
#define MAXBITS MAXBYTES*8

void encrypt(char* buf, char *key);
void decrypt(char* buf, char *key);

#endif /* !__RXE_H */
