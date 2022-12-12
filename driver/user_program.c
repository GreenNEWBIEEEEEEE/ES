#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    int bufsize = 1024;
    char buf[1024] = "";
    char buf2[1024] = "";

    scanf("%s", buf);
    
    int fd = open("/dev/demo", O_RDWR);
    if (fd < 0)
    {
        printf("open file failed %d\n", fd);
        return -1;
    }

    int ret = write(fd, buf, bufsize);

    if (ret < 0)
    {
        printf("write to driver failed\n");
        return -1;
    }

    ret = read(fd, buf2, bufsize);
    
    if (ret < 0)
    {
        printf("write to driver failed\n");
        return -1;
    }

    close(fd);
    printf("%s\n", buf2);

    return 0;
}

