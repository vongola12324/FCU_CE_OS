#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

void PANIC(char *msg) {
    perror(msg);
    exit(1);
}

void http(struct sockaddr_in *client_addr) {
    char buffer[1512];
    char port[10];
    char *argv[3] = {"./hello_cgi", "./hello_cgi", 0};
    read(0, buffer, sizeof(buffer));
    setenv("REMOTE_ADDR", inet_ntoa(client_addr->sin_addr), 1);
    sprintf(port, "%d", ntohs(client_addr->sin_port));
    setenv("REMOTE_PORT", port, 1);
    if (!fork())
        execvp(argv[0], argv);
}
int main(int argc, char *argv[]) {
    int sd;
    int port;
    int new_fd;
    int len;
    int state;
    struct sockaddr_in server_addr, client_addr;
    if (argc > 1)
        port = atoi(argv[1]);
    else
        port = 80;
    sd = socket(PF_INET, SOCK_STREAM, 0);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
        PANIC("Bind");
    if (listen(sd, 20) != 0)
        PANIC("Listen");
    while (1) {
        len = sizeof(client_addr);
        new_fd = accept(sd, (struct sockaddr *)&client_addr, (void *)&len);
        const int nfd = new_fd;
        printf("%d,Client from %s:%d\n", nfd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        if (new_fd < 0)
            perror("Accept");
        else {
            if (!fork()) {
                close(sd);
                dup2(nfd, 0);
                dup2(nfd, 1);
                dup2(nfd, 2);
                close(nfd);
                http(&client_addr);
                wait(&state);
                exit(0);
            }
            close(nfd);
        }
    }
    return 0;
}

