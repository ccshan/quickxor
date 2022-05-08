#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef unsigned char BYTE;

int quickxor(int fd) {
    BYTE buf[BUFSIZ];
    unsigned char shift = 0, bytes, bits;
    size_t length = 0;
    ssize_t count, i;
    BYTE block[20];

    memset(block, 0, sizeof(block));
    while (count = read(fd, buf, sizeof(buf))) {
        if (count < 0)
            if (errno == EAGAIN || errno == EWOULDBLOCK)
                continue;
            else
                return 0;
        for (i = 0; i < count; ++i) {
            BYTE c = buf[i];
            bytes = shift / 8;
            bits  = shift % 8;
            block[ bytes                   ] ^= c <<    bits ;
            block[(bytes+1) % sizeof(block)] ^= c >> (8-bits);
            shift = (shift + 11) % (sizeof(block) * 8);
        }
        length += count;
    }
    for (bytes = 12; length; length >>= 8)
        block[bytes++] ^= length;
    for (bytes = 0; bytes < sizeof(block); bytes++)
        printf("%02x", block[bytes]);
    printf("  ");
    return 1;
}

int main(int argc, const char *const argv[]) {
    int i, fd;
    if (argc <= 1) {
        if (quickxor(STDIN_FILENO))
            puts("-");
        else
            perror("-");
    } else {
        for (i = 1; i < argc; ++i) {
            if (0 <= (fd = open(argv[i], O_RDONLY))) {
                if (quickxor(fd))
                    puts(argv[i]);
                else
                    perror(argv[i]);
            } else {
                perror(argv[i]);
            }
        }
    }
    return 0;
}
