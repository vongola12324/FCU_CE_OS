 
#include <stdio.h>
#include <stdlib.h>
#define REMOTE_ADDR getenv("REMOTE_ADDR")
#define REMOTE_PORT getenv("REMOTE_PORT")

int print_environment() {
    printf("<B>REMOTE_PORT:</B> %s<BR>\n", REMOTE_PORT);
    printf("<B>REMOTE_ADDR:</B> %s<BR>\n", REMOTE_ADDR);
}
main(int argc, char *argv[]) {
    printf("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
    printf("<html><body>\n");
    printf("Hello CGI!<BR>\n");
    print_environment();
    printf("</body></html>\n\n");
}
