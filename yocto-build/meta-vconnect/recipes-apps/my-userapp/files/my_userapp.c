#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_PATH "/dev/my_device"
#define BUFFER_LENGTH 256

int main()
{
    int fd;
    char write_buf[BUFFER_LENGTH];
    char read_buf[BUFFER_LENGTH];

    printf("Opening %s...\n", DEVICE_PATH);
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return -1;
    }

    /* Send data to Kernel */
    printf("Enter a message to send to the kernel: ");
    if (fgets(write_buf, sizeof(write_buf), stdin) == NULL) {
        close(fd);
        return 0;
    }
    write_buf[strcspn(write_buf, "\n")] = 0; /* Remove trailing newline */

    printf("Writing message to driver...\n");
    ssize_t bytes_written = write(fd, write_buf, strlen(write_buf));
    if (bytes_written < 0) {
        perror("Failed to write to device");
        close(fd);
        return -1;
    }

    /* Read data back from Kernel */
    printf("Reading message back from driver...\n");
    ssize_t bytes_read = read(fd, read_buf, BUFFER_LENGTH - 1);
    if (bytes_read < 0) {
        perror("Failed to read from device");
        close(fd);
        return -1;
    }
    read_buf[bytes_read] = '\0';

    printf("Received from Kernel: [%s]\n", read_buf);

    close(fd);
    return 0;
}
