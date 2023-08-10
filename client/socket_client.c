#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {

    // create TCP socket
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        perror("Error");
        return 1;
    }
    printf("socket created\n");


    // create address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1337);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");


    // request to connect client socket to server socket at above address
    if (connect(sockfd, (const struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("Error");
        return 1;
    }
    printf("connection successful\n");


    // open file to write in binary mode
    FILE *imgfd = fopen("output.jpg", "wb");

    if (imgfd == NULL) {
        perror("Error");
        return 1;
    }
    printf("file created\n");


    // allocate memory for buffer
    char *img_buffer[1024];


    // receive data
    ssize_t received;

    while ((received = recv(sockfd, img_buffer, 1024, 0)) > 0) {
        fwrite(img_buffer, 1, 1024, imgfd);
        printf(".");
    }
    printf("\n");

    if (received == -1) {
        perror("Error");
        return 1;
    }
    printf("data received\n");


    // close file
    int closed = fclose(imgfd);

    if (closed != 0) {
        perror("Error");
        return 1;
    }
    printf("file closed\n");


    // close socket
    closed = close(sockfd);

    if (closed != 0) {
        perror("Error");
        return 1;
    }
    printf("socket closed\n");

    return 0;
}