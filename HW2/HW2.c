#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/wait.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <pthread.h>
#define SERVER_PORT 22222

void PANIC(char *msg) {
    perror(msg);
    exit(-1);
}

void *ServerThread(void *arg) {
    char buf[100];
    int bytes_read;
    int fd = *(int *)arg; //Get Client's FD
    for (;;) {
        bytes_read = recv(fd, buf, sizeof(buf), 0);
        send(fd, buf, bytes_read, 0);
        if (strncmp(buf, "q", 1) == 0)
            break;
    }
    printf("Connection Close!\n");
    close(fd);
    return arg;
}

int main(void)
{
    int fd;
    struct sockaddr_in server_addr;
    pthread_t my_thread;
    int client_fd, addr_size;
    if ((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        PANIC("Socket");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
        PANIC("bind");
    if (listen(fd, 20) != 0)
        PANIC("listen");
    for (;;) {
        addr_size = sizeof(server_addr);
        client_fd = accept(fd, (struct sockaddr *)&server_addr, (socklen_t *)&addr_size);
        printf("Connected: %s:%d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
        const int cfd = client_fd;
        if (pthread_create(&my_thread, NULL, ServerThread,(void *) &cfd) != 0)
            perror("Thread creation");
        else
            pthread_detach(my_thread);
    }
    return 0;
}
