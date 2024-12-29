#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "entity.h"

int messages = 0;
pthread_mutex_t messages_mutex = PTHREAD_MUTEX_INITIALIZER;

#define PORT 8080
#define BUFFER_SIZE 1024

struct Player 
    {
        float x;
        float y;
    };

/* Zadanie 2
 * Client wyswietla czerwony kwadrat na ekranie.
 * Tworzymy okno za pomoca rayliba i rysujemy kwadrat.
 *
 * Zadanie 3
 * Movement system:
 * Jakies przyciski przesuwaja kwadrat w jakis tam sposob
 * Na razie only client side
 *
 * Zadanie 4
 * Zamiast obslugiwac movement po stronie klienta, przenies state (pozycja itd.)
 * na serwer. Klient powinien wysylac wiadomosc do serwera ze chce poruszyc
 * kwadratem a server powinien to zarejestrowac, zmodyfikowac state i wyslac
 * aktualizacje stanu do clienta.
 */

void *handle_client(void *socket_desc) {
    
    struct Player plr;
    int sock = *(int *)socket_desc;
    char buffer[BUFFER_SIZE] = {0};

    while (1) {
        // Clear the buffer and wait for a response
        memset(buffer, 0, BUFFER_SIZE);
        int read_size = read(sock, buffer, BUFFER_SIZE);
        
        if (read_size <= 0) {
            printf("Client disconnected or error occurred\n");
            break;
        }
printf("%d\n", *buffer);
        // Print the response from the client
        if(strcmp(buffer, "up") == 0)
{
	printf("0\n");
    plr.y-=0.01;
}

    if(strcmp(buffer, "up-right") == 0)
    {
        printf("1\n");
        plr.y-= 0.01;
        plr.x+=0.01;
    }

    if(strcmp(buffer, "right") == 0)
    {
        printf("2\n");
        plr.x+= 0.01;
    }

    if(strcmp(buffer, "down-right") == 0)
    {
        printf("3\n");
        plr.y+=0.01;
        plr.x+=0.01;
    }

    if(strcmp(buffer, "down") == 0)
    {
        printf("4\n");
        plr.y+= 0.01;
    }

    if(strcmp(buffer, "up-left") == 0)
    {
        printf("-1\n");
        plr.y-= 0.01;
        plr.x-= 0.01;
    }

    if(strcmp(buffer, "left") == 0)
    {
        printf("-2\n");
        plr.x-= 0.01;
    }

    if(strcmp(buffer, "down-left") == 0)
    {
        printf("-3\n");
        plr.y+= 0.01;
        plr.x-= 0.01;
    }
    char newpos[20] = {"NewPos%f,%f",plr.x,plr.y};
    send(sock, newpos, strlen(newpos), 0);
}

    // Close the socket and free the thread
    close(sock);
    free(socket_desc);
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // pthread_mutex_init(&messages_mutex);

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Define the type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        // Accept a connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Create a new thread for each client
        pthread_t client_thread;
        int *new_sock = malloc(1);
        *new_sock = new_socket;
        if (pthread_create(&client_thread, NULL, handle_client, (void *)new_sock) < 0) {
            perror("could not create thread");
            free(new_sock);
            exit(EXIT_FAILURE);
        }

        // Detach the thread so it cleans up after itself
        pthread_detach(client_thread);
    }

    // Close the server socket (this line will never be reached in this example)
    close(server_fd);
    return 0;
}

