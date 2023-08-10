#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {

    // create TCP socket
    int server_sockfd = socket(PF_INET, SOCK_STREAM, 0); 

    if (server_sockfd == -1) {
        perror("Error");
        return 1;
    }
    printf("socket created\n");


    // create address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1337);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");


    // assign address to socket
    int bound = bind(server_sockfd, (const struct sockaddr *) &addr, sizeof(addr));

    if (bound != 0) {
        perror("Error");
        return 1;
    }
    printf("address assigned\n");


    // listen for connection request
    int listening = listen(server_sockfd, 1);

    if (listening != 0) {
        perror("Error");
        return 1;
    }
    printf("listening...\n");


    // accept connection request
    int client_sockfd = accept(server_sockfd, NULL, NULL);

    if (client_sockfd == -1) {
        perror("Error");
        return 1;
    }
    printf("request accepted\n");


    // open file to read in binary mode
    FILE *imgfd = fopen("adele.jpg", "rb");

    if (imgfd == NULL) {
        perror("Error");
        return 1;
    }
    printf("file opened\n");


    // go to end of file IOT get file size
    int sought = fseek(imgfd, 0, SEEK_END);

    if (sought != 0) {
        perror("Error");
        return 1;
    }

    // get file size
    long img_size = ftell(imgfd);

    if (img_size == -1) {
        perror("Error");
        return 1;
    }
    printf("file size determined\n");


    // allocate memory for buffer
    char *img_buffer = malloc(img_size);

    if (img_buffer == NULL) {
        perror("Error");
        return 1;
    }
    printf("memory allocated\n");


    // go back to beginning of file IOT read file into buffer
    sought = fseek(imgfd, 0, SEEK_SET);

    if (sought != 0) {
        perror("Error");
        return 1;
    }

    // read file into buffer
    fread(img_buffer, 1, img_size, imgfd);

    // check for errors
    int read = ferror(imgfd);

    if (read != 0) {
        perror("Error");
        return 1;
    }
    printf("file read into memory\n");


    // close file
    int closed = fclose(imgfd);

    if (closed != 0) {
        perror("Error");
        return 1;
    }
    printf("file closed\n");


    // send data
    ssize_t sent;
    while((img_size > 0) && ((sent = send(client_sockfd, img_buffer, 1024, 0)) > 0)) {
        img_buffer += 1024;
        img_size -= sent;
        printf(".");
    }
    printf("\n");
    
    if (sent == -1) {
        perror("Error");
        return 1;
    }
    printf("data sent\n");


    // close sockets
    int server_closed = close(server_sockfd);
    int client_closed = close(client_sockfd);

    if (server_closed != 0 || client_closed != 0) {
        perror("Error");
        return 1;
    }
    printf("sockets closed\n");

    return 0;
}