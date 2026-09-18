#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE "/dev/my_gpio"

int main(int argc, char *argv[])
{
    int fd;
    char value;

    if (argc != 2) {
        printf("Usage: %s <0|1>\n", argv[0]);
        return 1;
    }

    if (argv[1][0] != '0' && argv[1][0] != '1') {
        printf("Invalid value. Use 0 or 1.\n");
        return 1;
    }

    value = argv[1][0];

    fd = open(DEVICE, O_WRONLY);

    if (fd < 0) {
        perror("Failed to open /dev/my_gpio");
        return 1;
    }

    if (write(fd, &value, 1) != 1) {
        perror("Failed to write GPIO value");
        close(fd);
        return 1;
    }

    close(fd);

    printf("GPIO set to %c\n", value);

    return 0;
}
