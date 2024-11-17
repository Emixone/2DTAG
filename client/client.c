#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <raylib.h>
#define MAX_SPEED 0.5f
#define SPEED_INCREMENT 0.01f
#define PORT 8080
#define BUFFER_SIZE 1024
typedef struct
{
    int x;
    int y;
}Direction;
Direction direction;
float speedX = 0;
float speedY = 0;
float positionX = 0;
float positionY = 0;
int messages = 0;
bool isMoving;
float clamp(float value, float min, float max)
{
    if (value < min)
    {
        return min;
    }
    else if (value > max)
    {
        return max;
    }
    else
    {
        return value;
    }

}
int
main()
{
    InitWindow(800,400, "2DTAG");
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        // A - obliczenie kierunku
        direction.x = 0;
        direction.y = 0;
        direction.x += IsKeyDown(KEY_D);
        direction.x -= IsKeyDown(KEY_A);
        direction.y += IsKeyDown(KEY_S);
        direction.y -= IsKeyDown(KEY_W);

        // B - obliczenie predkosci
        speedX = clamp(direction.x * SPEED_INCREMENT + speedX, -MAX_SPEED, MAX_SPEED);
        speedY = clamp(direction.y * SPEED_INCREMENT + speedY, -MAX_SPEED, MAX_SPEED);
        if(direction.x == 0)
        {
            speedX = 0;
        }
        if(direction.y == 0)
        {
            speedY = 0;
        }
        if (direction.x != 0 && direction.y != 0)
        {
            speedX = sqrt((speedX * speedX) + (speedY * speedY));
            speedY = sqrt((speedX * speedX) + (speedY * speedY));
        }

        // C - aktualizacja pozycji
        positionX += speedX;
        positionY += speedY;
            
         
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

