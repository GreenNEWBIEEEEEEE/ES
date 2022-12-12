#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
    int bufsize = 1024;
    char buf[1024] = "";  // input
    char buf2[1024] = ""; // output

    scanf("%s", buf);     // input string
    
    int fd = open("/dev/demo", O_RDWR);  // open device
    if (fd < 0)   // If opening successfully, it will return 0
    {
        printf("open file failed %d\n", fd);
        return -1;
    }

    int ret = write(fd, buf, bufsize);    // write input buffer into device.

    if (ret < 0)
    {
        printf("write to driver failed\n");
        return -1;
    }

    ret = read(fd, buf2, bufsize);    // read output buffer from device.
    
    if (ret < 0)
    {
        printf("write to driver failed\n");
        return -1;
    }

    close(fd);
    printf("%s\n", buf2);

    return 0;
}

