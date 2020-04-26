#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h> 

int main()
{
    struct stat st;
    if(-1 == stat("hole.txt", &st))
    {
        printf("get file stat fail \n");
        return -1;
    }

    //printf("\n",);
    printf("设备ID：%d\n", st.st_dev);
    printf("文件节点：%d\n", st.st_ino);
    printf("文件的保护模式： %d\n", st.st_mode);

    return 0;
}

