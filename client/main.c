#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#define DEFAULT_BUFLEN 65536
#include "client.h"

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "Russian");
    char ip[16], port[6] = "23", *msg, *buffer;
    msg = malloc(DEFAULT_BUFLEN);
    buffer = malloc(DEFAULT_BUFLEN);
    if (!msg || !buffer) {
        printf("No free memory.\n");
        return 1;
    }
    if (argc >= 4) {
        strcpy(ip, argv[1]);
        strcpy(port, argv[2]);
        strcpy(msg, argv[3]);
    } else if (argc == 3) {
        strcpy(ip, argv[1]);
        strcpy(port, argv[2]);
        printf("Message: ");        
        gets_s(buffer, DEFAULT_BUFLEN);
        strcpy(msg, buffer);
    } else if (argc == 2) {
        strcpy(ip, argv[1]);
        printf("Port (default=23): ");
        gets_s(buffer, 5);
        sscanf(buffer, "%s", port);
        printf("Message: ");        
        gets_s(buffer, DEFAULT_BUFLEN);
        strcpy(msg, buffer);
    } else {
        printf("IP address: ");
        gets_s(buffer, 15);
        sscanf(buffer, "%s", ip);
        printf("Port (default=23): ");
        gets_s(buffer, 5);
        sscanf(buffer, "%s", port);
        printf("Message: ");        
        gets_s(buffer, DEFAULT_BUFLEN);
        strcpy(msg, buffer);
    }
    int res = client(ip, port, msg);
    free(buffer);
    free(msg);
    return res;
}

