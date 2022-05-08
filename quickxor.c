#include <stdio.h>
#include <string.h>

int main() {
    unsigned char shift = 0, bytes, bits;
    size_t length = 0;
    int c;
    unsigned char block[20];

    memset(block, 0, sizeof(block));
    while (EOF != (c = getchar())) {
        bytes = shift / 8;
        bits  = shift % 8;
        block[ bytes                   ] ^= (unsigned char)c <<    bits ;
        block[(bytes+1) % sizeof(block)] ^= (unsigned char)c >> (8-bits);
        length++;
        shift = (shift + 11) % (sizeof(block) * 8);
    }

    for (bytes = 12; length; length >>= 8)
        block[bytes++] ^= length;

    for (bytes = 0; bytes < sizeof(block); bytes++)
        printf("%02x", block[bytes]);
    printf("  ");
    return 0;
}
