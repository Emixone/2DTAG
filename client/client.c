#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <raylib.h>
#include "entity.h"

#define PORT 8080
#define BUFFER_SIZE 1024

Entity player;

int messages = 0;
bool isMoving;

int
main()
{
    InitWindow(800,400, "2DTAG");
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        // A - obliczenie kierunku
        player.direction.x = 0;
        player.direction.y = 0;
        player.direction.x += IsKeyDown(KEY_D);
        player.direction.x -= IsKeyDown(KEY_A);
        player.direction.y += IsKeyDown(KEY_S);
        player.direction.y -= IsKeyDown(KEY_W);

        Entity_move(&player);

        /*1. Poprawic bug z predkoscia rosnaca w nieskonczonosc dla ujemnych SpeedX i SpeedY
        2. Poczytac co to jest rownanie pitagorsa
        3. Poczytac co toj est matematyczny wektor
        2a. Dlaczego jak wcisniemy D+S ruszamy sie szybciej? (ma zwiazek z rownaniem pitagorasa) */
        DrawRectangle(player.position.x, player.position.y, 50, 50, RED);
        printf("%f  %f\n", player.speed.x, player.speed.y);
        EndDrawing();
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

