#ifndef __CLIENT_H_INC
#define __CLIENT_H_INC

char *gets_s(char *s, int n) {
    fflush(stdin);
    int i, k = getchar();
    if (k == EOF)
         return NULL;
    for (i = 0; (k != EOF && k != '\n') && n--; i++) {
        s[i] = k;
        k = getchar();
        if (k == EOF && !feof(stdin))
            return NULL;
    }
    s[i] = '\0';
    return s;    
}

int client(const char *ip, const char *port, char *msg) {
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    char *sendbuf;
    char *recvbuf = calloc(DEFAULT_BUFLEN, 1);
    int iResult, i;
    int recvbuflen = DEFAULT_BUFLEN;
    sendbuf = msg;
    /* initialize winsock API */
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult) {
        printf("WSAStartup failed with error %d\n", iResult);
        free(recvbuf);
        return 1;
    }
    
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    /* Resolve the server address and port */
    iResult = getaddrinfo(ip, port, &hints, &result);
    if (iResult) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        free(recvbuf);
        return 1;
    }
    
    /* Attempt to connect to an address until one succeeds */
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        /* Create a SOCKET for connecting to server */
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %d\n", WSAGetLastError());
            WSACleanup();
            free(recvbuf);
            return 1;
        }
        /* connect to server */
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);
    
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        free(recvbuf);
        return 1;
    }
    
    /* Send an initial buffer */
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        free(recvbuf);
        return 1;
    }
    
    printf("Bytes sent: %ld\n", iResult);
    
    /* shutdown the connection since no more data will be sent */
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        free(recvbuf);
        return 1;
    }
    
    /* Receive until the peer closes the connection */
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            recvbuf[iResult] = '\0';
            printf("Server answer: \"%s\"\n", recvbuf);
        } else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());
    } while (iResult > 0);
   
    /* cleanup */
    free(recvbuf);
    closesocket(ConnectSocket);
    WSACleanup();
}

#endif
