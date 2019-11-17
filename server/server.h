#ifndef __SERVER_H_INC
#define __SERVER_H_INC

/*#define _WIN32_WINNT 0x501*/
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <sys/stat.h>
#include <signal.h>
#define MAX_USER_INFO_LEN 0x800
#include "serversys.h"

typedef unsigned char byte;

FILE *logFileG;
static bool notification = false;
void server_kill(void);
int server_init(void);

void showLog(const char *msg, int iip, FILE *f) {
    char *ip = makeIP(iip);
    char *time = timeStr();
    fprintf(f, "[%s] msg from: %15s | %s\n", time, ip, msg); 
    free(ip);
    free(time);
}

void server_kill() {
    fclose(logFileG);
}

void sigint(int sig) {
    LogEvent("Interrupted (SIGINT).");
    server_kill();
    exit(0);
}
void sigsegv(int sig) {
    LogEvent("Null pointer (SIGSEGV).");
    server_kill();
    exit(4);
}

int server_init() {
    signal(SIGSEGV, sigsegv);
    signal(SIGINT, sigint);
    setlocale(LC_ALL, "Russian");
    static char logname[20] = {};
    time_t t = time(NULL);
    struct tm *tmCl = localtime(&t);
    sprintf(logname, "log_%s%d_%s%d_%d.txt",
        isTwoSgn(tmCl->tm_mday), tmCl->tm_mday,
        isTwoSgn(tmCl->tm_mon), tmCl->tm_mon + 1, tmCl->tm_year + 1900);
    logFileG = fopen(logname, "a");
    if (!logFileG) {
        logFileG = fopen(logname, "w");
    }
    return 0;
}

int basic_server(const char *ipAddr, const char *port) {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    char *time = timeStr();
    if (!time)
        time = " Error in timeStr()";
    fprintf(logFileG, "[%s] Initializing... (IP=%s, Port=%s)\n", time, ipAddr, port);
    if (result) {
        fprintf(logFileG, "[%s] WSAStartup(...) failed: %d\n", time, result);
        server_kill();
        return result;
    }
    struct addrinfo *addr = NULL;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    result = getaddrinfo(ipAddr, port, &hints, &addr);

    if (result != 0) {
        fprintf(logFileG, "[%s] getaddrinfo(...) failed: %d\n", time, result);
        WSACleanup();
        server_kill();
        return 1;
    }

    int listen_socket = socket(addr->ai_family, addr->ai_socktype,
        addr->ai_protocol);
    if (listen_socket == INVALID_SOCKET) {
        fprintf(logFileG, "[%s] Error at socket: %d\n", time, WSAGetLastError());
        freeaddrinfo(addr);
        WSACleanup();
        server_kill();
        return 1;
    }

    result = bind(listen_socket, addr->ai_addr, (int)addr->ai_addrlen);

    if (result == SOCKET_ERROR) {
        fprintf(logFileG, "[%s] bind(...) failed with error: %d\n", time, WSAGetLastError());
        freeaddrinfo(addr);
        closesocket(listen_socket);
        WSACleanup();
        server_kill();
        return 1;
    }

    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) {
        fprintf(logFileG, "[%s] listen(...) failed with error: %d\n",
            time, WSAGetLastError());
        closesocket(listen_socket);
        WSACleanup();
        server_kill();
        return 1;
    }


    int client_socket = INVALID_SOCKET;
    char *user_info = (char*)malloc(MAX_USER_INFO_LEN); /* request of client */
    char *response = (char*)malloc(1 << 15);
    response[0] = 0;
    if (!response) {
        LogError("No free memory.");
        closesocket(listen_socket);
        WSACleanup();
        server_kill();
        return 1;
    }
    
    fprintf(logFileG, "[%s] Run (IP=%s, Port=%s)\n", time, ipAddr, port);
    

    for (;;) {
        client_socket = accept(listen_socket, NULL, NULL);
                
        if (client_socket == INVALID_SOCKET) {
            fprintf(stderr, "accept(...) failed: %d\n", WSAGetLastError());
            closesocket(listen_socket);
            WSACleanup();
            server_kill();
            return 1;
        }

        result = recv(client_socket, user_info, MAX_USER_INFO_LEN, 0);

        if (result == SOCKET_ERROR) {
            fprintf(logFileG, "[%s] recv(...) failed: %d\n", time, result);
            closesocket(client_socket);
        } else if (result == 0) {
            LogError("Connection closed.");
        } else if (result > 0) {
            user_info[result] = '\0';
            
            struct sockaddr_in testdata = {};
            socklen_t socklen = sizeof(testdata);
            getpeername(client_socket, (struct sockaddr *)&testdata, &socklen);
            unsigned int Iip = testdata.sin_addr.s_addr;

            showLog(user_info, Iip, logFileG);
            showLog(user_info, Iip, stdout);            
            sprintf(response, "Got your message: %s", user_info);                         

          SendAnswer:
            result = send(client_socket, response, strlen(response), 0);
            if (notification) MsgMelody();

            if (result == SOCKET_ERROR) {
                fprintf(stderr, "send(...) failed: %d\n", WSAGetLastError());
            }
            closesocket(client_socket);
        }
    }
    free(response);
    free(user_info);
    free(time);
    closesocket(listen_socket);
    freeaddrinfo(addr);
    WSACleanup();
    return 0;
}

#endif

