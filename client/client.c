#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <raylib.h>
#include "entity.h"
#include <bits/pthreadtypes.h>

#define PORT 8080
#define BUFFER_SIZE 1024

/*
1. Kiedy client sie polaczy, serwer musi stworzyc strukt Player z pozycja 0, 0
2. Kiedy serwer dostanie wiadomosc z direction, np. "left", "right" itd. aktualizuje pozycje Playera (podobnie jak to sie dzieje teraz na kliencie)
3. Kiedy serwer zakutalizuje pozycje, musi wyslac wiadomosc NewPos x,y 
*/

Entity player;

int messages = 0;
bool isMoving;
int handle_server(void)
{
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

        if(player.direction.y == -1 && player.direction.x == 0)
        {
	        send(sock, "up", strlen("up"), 0);
        }
        else if(player.direction.y == -1 && player.direction.x == 1)
        {
            send(sock, "up-right", strlen("up-right"), 0);
        }
        else if(player.direction.x == 1 && player.direction.y == 0)
        {
            send(sock, "right", strlen("right"), 0);
        }
        else if(player.direction.x == 1 && player.direction.y == 1)
        {
            send(sock, "down-right", strlen("down-right"), 0);
        }
        else if(player.direction.y == 1 && player.direction.x == 0)
        {
            send(sock, "down", strlen("down"), 0);
        }
        else if(player.direction.x == -1 && player.direction.y == -1)
        {
            send(sock, "up-left", strlen("up-left"), 0);
        }
        else if(player.direction.x == -1 && player.direction.y == 0)
        {
            send(sock, "left", strlen("left"), 0);
        }
        else if(player.direction.y == 1 && player.direction.x == -1 )
        {
            send(sock, "down-left", strlen("down-left"), 0);
        }
    }

    // Close the socket
    close(sock);
    return 0;
}

int
main()
{
    player.direction.x = 0;
    player.direction.y = 0;
    pthread_t client_thread;
    if (pthread_create(&client_thread, NULL, handle_server, (void *)NULL) < 0) {
            perror("could not create thread");
            exit(EXIT_FAILURE);
        }

        // Detach the thread so it cleans up after itself
        pthread_detach(client_thread);
 
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

        
        DrawRectangle(player.position.x, player.position.y, 50, 50, RED);
        
        EndDrawing();
    }

    return 0;
}

