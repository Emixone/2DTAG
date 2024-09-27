#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <raylib.h>

#define PORT 8080
#define BUFFER_SIZE 1024
int positionX;
int positionY;
int messages = 0;
int
main()
{
    InitWindow(800,400, "2DTAG");
    while(!WindowShouldClose())
    {
        if(IsKeyDown(KEY_D))
        {
           positionX++;
        }
        if(IsKeyDown(KEY_A))
        {
           positionX--;
        }
        if(IsKeyDown(KEY_W))
        {
           positionY++;
        }
        if(IsKeyDown(KEY_S))
        {
           positionY--;
        }
        DrawRectangle(positionX, positionY, 50, 50, RED);
    }
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    while (1) {
        // Clear the buffer and wait for a message from the server
        memset(buffer, 0, BUFFER_SIZE);

        send(sock, "pong", strlen("pong"), 0);
        messages++;

	usleep(100);
        printf("Sent to server: pong\n");
        printf("Messages sent: %d\n", messages);
    }

    // Close the socket
    close(sock);
    return 0;
}
