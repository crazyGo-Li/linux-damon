#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
    int fd = -1;
    ssize_t size = -1;
    off_t offset = -1;
    char buf1[] = "123456";
    char buf2[] = "afdhjk";

    char filename[] = "hole.txt";

    fd = open(filename, O_RDWR | O_CREAT | S_IRWXU);
    if(fd < 0)
    {
        printf("open file fail\n");
        close(fd);
        return -1;
    }

    size = write(fd, buf1, sizeof(buf1));
    if(size != sizeof(buf1))
    {
        printf("write buf1 fail\n");
        close(fd);
        return -2;
    }

    offset = lseek(fd, 32, SEEK_SET);
    if(-1 == offset)
    {
        printf("set lseek fail\n");
        close(fd);
        return -3;
    }

    size = write(fd, buf2, sizeof(buf2));
    if(size != sizeof(buf2))
    {
        printf("write buf2 fail\n");
        close(fd);
        return -4;
    }

    close(fd);

    return 0;
}

