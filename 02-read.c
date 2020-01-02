#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int fd = -1;
    char filename[] = "test.txt";
    ssize_t size = -1;
    char buf[10];
    int i = 0;
        
    fd = open(filename, O_RDWR);
    if (-1 == fd)
    {
        printf("open %s fail, fd : %d\n", filename, fd);
        return 0;
    }
    else
    {
        printf("open %s fail, fd : %d\n", filename, fd);
    }

    while(size)
    {
        size = read(fd, buf, 10);
        if (-1 == size)
        {
            printf("read file error occurs\n");
            close(fd);
            return -1;
        }
        else
        {
            if (size > 0)
            {
                printf("read %d bytes:", size);
                printf("\"");
                for (i = 0; i < size; i ++)
                {
                    printf("%c", *(buf+i));
                }

                printf("\"\n");
                
            }
            else
            {
                printf("reatch the end of file \n");
                close(fd);
                return 0;
            }
        }
    }
}
