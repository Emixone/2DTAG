#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <raylib.h>
#define MAX_SPEED 0.5f
#define SPEED_INCREMENT 0.01f
#define PORT 8080
#define BUFFER_SIZE 1024
float speedX = 0;
float speedY = 0;
float positionX = 0;
float positionY = 0;
int messages = 0;
bool isMoving;
    void moveRight()
    {
        speedX += SPEED_INCREMENT;
        if (speedX > MAX_SPEED)
        {
            speedX = MAX_SPEED;
        }
        positionX += speedX;
    }
    void moveLeft()
    {
        speedX -= SPEED_INCREMENT;
        if (speedX > MAX_SPEED)
        {
            speedX = MAX_SPEED;
        }
        positionX += speedX;
    }
    void moveUp()
    {
        speedY -= SPEED_INCREMENT;
        if (speedY > MAX_SPEED)
        {
            speedY = MAX_SPEED;
        }
        positionY += speedY;
    }
    void moveDown()
    {
        speedY += SPEED_INCREMENT;
        if (speedY > MAX_SPEED)
        {
            speedY = MAX_SPEED;
        }
        positionY += speedY;
    }
int
main()
{
    InitWindow(800,400, "2DTAG");
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        if(IsKeyDown(KEY_D))
        {
           moveRight();
           isMoving = true;
        }
        else if(IsKeyDown(KEY_A))
        {
            moveLeft();
        }
        else
        {
            speedX = 0;
        }
        if(IsKeyDown(KEY_W))
        {
           moveUp();
        }
        else if(IsKeyDown(KEY_S))
        {
           moveDown();
        }
        else
        {
            speedY = 0;
        }
        /*1. Poprawic bug z predkoscia rosnaca w nieskonczonosc dla ujemnych SpeedX i SpeedY
        2. Poczytac co to jest rownanie pitagorsa
        3. Poczytac co toj est matematyczny wektor
        2a. Dlaczego jak wcisniemy D+S ruszamy sie szybciej? (ma zwiazek z rownaniem pitagorasa) */
        DrawRectangle(positionX, positionY, 50, 50, RED);
        printf("%f  %f\n",speedX,speedY);
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

