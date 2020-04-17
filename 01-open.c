#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd = -1;
    char filename[] = "test.txt";
    
    fd = open(filename, O_RDWR | O_CREAT | O_EXCL | S_IRWXU);
    if (-1 == fd)
    {
        printf("open file %s error!, fd : %d\n", filename, fd);
        
    }
    else
    {
        printf("open file %s success, fd : %d\n", filename, fd);
        close(fd);
        
    }
    return 0;
}
