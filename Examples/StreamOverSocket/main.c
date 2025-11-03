#include <stdio.h>
#include <winsock2.h>
#include <pthread.h>
#include "stdint.h"
#include <windows.h>


#pragma comment(lib, "ws2_32.lib")

void setup();
void loop();



void* LoopThread(void *args)
{
    int sleepIndexer=0;
    while (1)
    {
        loop();

        sleepIndexer++;
        if(sleepIndexer == 1000)
        {
            sleepIndexer = 0;
            Sleep(1);
        }
    }
    pthread_exit(NULL);
}


int main()
{
    WSADATA wsa;

//    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Initializing Winsock Failed. Error Code: %d\n", WSAGetLastError());
        return 0;
    }

//    printf("Initialized.\n");
    setup();


    pthread_t loopThread;

    // Create producer and consumer threads
    int ret =pthread_create(&loopThread, NULL, LoopThread, NULL);
    if(ret != 0){
        printf("create loopThread Failed. Error Code: %d\n",ret);
    }
    else {
        pthread_join(loopThread, NULL);
    }
    // Close the socket
    // closesocket(s);
    WSACleanup();

    return 0;
}
