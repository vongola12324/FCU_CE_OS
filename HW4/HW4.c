#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PANIC(msg) {
    perror(msg);
    exit(0);
}

#define STDIN 0
int main(int argc, char *argv[]) {
    int server_fd;
    int client_fd;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    fd_set master_fds;
    fd_set read_fds;
    int fdmax;
    struct timeval tv;
    int port;
    int len;
    int i, j;
    char buf[256], ch;
    int nbytes;
    int yes = 1;
    int retval;

    if (argc > 1)
        port = atoi(argv[1]);
    else
        port = 9999;
    tv.tv_sec = 60;
    tv.tv_usec = 0;

    FD_ZERO(&master_fds);
    FD_ZERO(&read_fds);
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        PANIC("socket");
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        PANIC("setsockopt");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        PANIC("bind");
    if (listen(server_fd, 10) == -1)
        PANIC("listen");
    FD_SET(server_fd, &master_fds);
    FD_SET(STDIN, &master_fds);

    fdmax = server_fd;
     
    for (;;) {
        read_fds = master_fds;
        retval = select(fdmax + 1, &read_fds, NULL, NULL, &tv);
        switch (retval) {
            case -1 : perror("select");
            continue;
            case 0:
            printf("Time Out...");
            for (i = 3; i <= fdmax; i++) {
                if (FD_ISSET(i, &master_fds))
                    close(i);
            }
              exit(0);
        }
        for (i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) {
                  if (i == server_fd) { //Handle New Connection
                    len = sizeof(client_addr);
                    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len)) == -1) {
                        perror("accept");
                        continue;
                    }
                    else {
                        FD_SET(client_fd, &master_fds);
                          if (client_fd > fdmax) {
                              fdmax = client_fd;
                        }
                        printf("New connection from %s on socket %d\n", inet_ntoa(client_addr.sin_addr), client_fd);
                    }
                }
                else { // Handle data from the clients and the STDIN
                    if ((nbytes = read(i, buf, sizeof(buf))) > 0) {
                        write(0, buf, nbytes);
                        for (j = 0; j <= fdmax; j++) {
                            if (FD_ISSET(j, &master_fds)) {
                                if (j != server_fd && j != i && j != 0) {
                                    if (send(j, buf, nbytes, 0) == -1) {
                                        perror("send");
                                    }
                                }
                            }
                        } //for
                    }
                    else {
                        perror("recv");
                        close(i);
                        FD_CLR(i, &master_fds); 
                    }
                }
            } //if
        }     //for scan all IO
    }         //for server loop
    return 0;
}
