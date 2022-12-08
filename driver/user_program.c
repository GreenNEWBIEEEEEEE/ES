#include <stdio.h>
#include <fcntl.h>

int main()
{
    int bufsize = 1024;
    char buf[bufsize];

    int fd = open("/dev/driver", O_RDWR);

    int ret = write(fd, buf, bufsize);
    
    ret = read(fd, buf, bufsize);

    close(fd);

    return 0;
}

