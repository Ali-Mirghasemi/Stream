#include <winsock2.h>
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include <pthread.h>

// bool dumpData = false;
typedef struct {
    SOCKET sock;
    struct sockaddr_in server;
    volatile bool isRunnig;
    const char *name;

    pthread_t sendThread;
    void *txBuffer;
    pthread_cond_t txCond;
    pthread_mutex_t txMutex;
    size_t inTx;
    void (*txComp)(void *);


    pthread_t recThread;
    uint8_t *rxBuffer;
    pthread_cond_t rxCond;
    pthread_mutex_t rxMutex;
    size_t inRx;
    void (*rxComp)(void *);



    void *args;
} UARTStreamOverSocket;

void *SendThread(void *args) {
    UARTStreamOverSocket *stream = (UARTStreamOverSocket *) args;
    SOCKET s = stream->sock;
    while (1) {
        pthread_mutex_lock(&stream->txMutex);
        while (stream->inTx == 0) {
            pthread_cond_wait(&stream->txCond, &stream->txMutex);
        }
        while (stream->inTx != 0) {
            int sent = send(s, stream->txBuffer, stream->inTx, 0);
            if (send < 0) {
                printf("%s->Send Failed.Error Code: %d .Exiting...\n", stream->name, sent);
                exit(0);
            }
            stream->inTx -= sent;
            stream->txBuffer += sent;
        }
        pthread_mutex_unlock(&stream->txMutex);

        if (stream->txComp != NULL) {
            stream->txComp(stream->args);
        }
    }
}

// Function executed by the receiving thread
void *ReceiveThread(void *args) {
    UARTStreamOverSocket *stream = (UARTStreamOverSocket *) args;
    SOCKET socket = stream->sock;
    int recv_size;

    while (1) {
        pthread_mutex_lock(&stream->rxMutex);
        while (stream->inRx == 0) {
            pthread_cond_wait(&stream->rxCond, &stream->rxMutex);
        }
        while(stream->inRx != 0) {
            recv_size = recv(socket, (char *) stream->rxBuffer, stream->inRx, 0);
            if (recv_size > 0) {
                stream->inRx -= recv_size;
                stream->rxBuffer += recv_size;
            }
            else {
                printf("%s->disconnected.Exiting...\n", stream->name);
                exit(0);
                break;
            }
        }
        pthread_mutex_unlock(&stream->rxMutex);

        if (stream->rxComp != NULL) {
            stream->rxComp(stream->args);
        }

    }
}

size_t socketinReceive(void *obj)
{
    UARTStreamOverSocket *stream = (UARTStreamOverSocket *)obj;
    return stream->inRx;
}

void socketReceive(void *obj, uint8_t *buffer, size_t len) {
    UARTStreamOverSocket *stream = (UARTStreamOverSocket *) obj;
    pthread_mutex_lock(&stream->rxMutex);

    // Set the data and signal the sending thread

    stream->rxBuffer = buffer;
    stream->inRx = len;

    pthread_cond_signal(&stream->rxCond);
    pthread_mutex_unlock(&stream->rxMutex);
}

void socketTransmit(void *obj, uint8_t *buffer, size_t len) {
    UARTStreamOverSocket *stream = (UARTStreamOverSocket *) obj;
    pthread_mutex_lock(&stream->txMutex);

    // Set the data and signal the sending thread

    stream->txBuffer = buffer;
    stream->inTx = len;

    pthread_cond_signal(&stream->txCond);
    pthread_mutex_unlock(&stream->txMutex);

}

void *socketInit(void *args, const char *ip, uint16_t port, const char *name, void (*txComp)(void *),
                 void (*rxComp)(void *)) {
    UARTStreamOverSocket *stream = malloc(sizeof(UARTStreamOverSocket));
    if (stream == NULL) {
        printf("%s->cannot alloc memory\n", name);
        return NULL;
    }
    // stream->HUART = huart;
    stream->name = name;
    stream->isRunnig = false;
    stream->txComp = txComp;
    stream->rxComp = rxComp;
    stream->args = args;
    stream->inTx = stream->inRx = 0;
    // Create a socket
    if ((stream->sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("%s->Could not create socket : %d\n", stream->name, WSAGetLastError());
        return NULL;
    }
//    printf("%s->Socket created.\n", stream->name);

    stream->server.sin_addr.s_addr = inet_addr(ip); // Replace with the IP address of your server
    stream->server.sin_family = AF_INET;
    stream->server.sin_port = htons(port); // Replace with the port number of your server

    // Connect to remote server
    if (connect(stream->sock, (struct sockaddr *) &stream->server, sizeof(stream->server)) < 0) {
        printf("%s->Connect error\n", stream->name);
        return NULL;
    }

    printf("%s->Connected\n", stream->name);

    stream->rxCond = PTHREAD_COND_INITIALIZER;
    stream->rxMutex = PTHREAD_MUTEX_INITIALIZER;
    int ret = pthread_create(&stream->recThread, NULL, ReceiveThread, stream);
    if (ret != 0) {
        printf("%s->Failed to create ReceiveThread.Error Code: %d\n", stream->name, ret);
        return NULL;
    }

    stream->txCond = PTHREAD_COND_INITIALIZER;
    stream->txMutex = PTHREAD_MUTEX_INITIALIZER;
    ret = pthread_create(&stream->sendThread, NULL, SendThread, stream);
    if (ret != 0) {
        printf("%s->Failed to create SendThread.Error Code: %d\n", stream->name, ret);
        pthread_cancel(stream->recThread);
        // Wait for the created thread to finish (note that it may not terminate immediately)
        pthread_join(stream->recThread, NULL);
        return NULL;
    }
    Sleep(1);
    stream->isRunnig = true;
    return stream;
}

void socketDeInit(void *obj) {
    closesocket(((UARTStreamOverSocket *) obj)->sock);
    free(obj);
}
