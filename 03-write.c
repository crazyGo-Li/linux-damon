#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(void)
{
    char buf[] = "hello world!";
    int fd = -1;
    ssize_t size = -1;
    char filename[] = "test.txt";

    fd = open(filename, O_RDWR);
    if (fd == -1)
    {
        printf("open file %s fail, fd : %d\n", filename, fd);
        return -1;
    }
    else
    {
        printf("open file %s success, fd : %d\n", filename, fd);
    }

    write(fd, buf, strlen(buf));

    close(fd);
}
