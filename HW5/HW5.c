#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
    int pid = getpid(), i;
    for(i=0; i<3; i++) {
        if (pid > 0) {
            pid = fork();
        }
        if (pid > 0) {
            printf("I'm the parent! pid = %d\n", getpid());
        } else if (pid == 0) {
            printf("I'm the child! pid = %d\n", getpid());
        } else {
            perror("Error!");
        }
    }
    return 0;
}
