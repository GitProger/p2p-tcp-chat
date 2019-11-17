#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <locale.h>
#include "server.h"

int main(int argc, char *argv[]) {
    server_init();
    char ip[16] = "127.0.0.1", port[6] = "23", buffer[16];
    printf(" === TCP Server === \n");
    if (argc >= 4) {
        strcpy(ip, argv[1]);
        strcpy(port, argv[2]);
        if (!stricmp(argv[3], "y"))
            notification = true;
        else 
            notification = false;        
    } else if (argc == 3) {
        strcpy(ip, argv[1]);
        strcpy(port, argv[2]);
    } else if (argc == 2) {
        strcpy(ip, argv[1]);
        printf("Port (default=%s): ", port);
        gets_s(buffer, 5);
        sscanf(buffer, "%s", port);
    } else {
        printf("IP address (default=%s): ", ip);
        gets_s(buffer, 15);
        sscanf(buffer, "%s", ip);
        printf("Port (default=%s): ", port);
        gets_s(buffer, 5);
        sscanf(buffer, "%s", port);
    } 
    printf("Server started. IP=%s | Port=%s\n", ip, port);
    return basic_server(ip, port);
}
