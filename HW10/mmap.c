#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void handle_error(msg) {

    do { 
        perror(msg);
        exit(EXIT_FAILURE);  
    } while (0);
}

int main(int argc, char *argv[])
{
    const char *memblock;
    int fd;
    struct stat sb;

    fd = open(argv[1], O_RDONLY);
    fstat(fd, &sb);
    printf("Size: %lu\n", (uint64_t)sb.st_size);

    memblock = mmap(NULL, sb.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (memblock == MAP_FAILED) handle_error("mmap");

    for(uint64_t i = 0; i < 10; i++)
    {
        printf("[%lu]=%X ", i, memblock[i]);

    }
    printf("\n");
    return 0;

}
