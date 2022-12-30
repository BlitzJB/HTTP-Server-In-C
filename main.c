#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void mainloop();
void WS_die();
void mainloop();
void getHTML(char *path, char *html);
void buildResponse(char *buffer, char *response);
char* getPath(char *buffer);
char* getTemplate(char *path);

WSADATA wsaData;
SOCKET server_fd, connection;
struct sockaddr_in address;
char buffer[BUFFER_SIZE] = {0};

struct RouteMap {
    char *path;
    char *template_name;
} routes[] = {
    {"/", "index.html"},
    {"/about", "about.html"},
    {"/contact", "contact.html"}
};;



void main() {
    // Winsock init
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) WS_die();

    // Socket create
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) WS_die();

    address.sin_family = AF_INET; // Address family = Internet
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP address
    address.sin_port = htons(PORT); // Port in network byte order (ie, converting it form an integer to address of the port)

    // Binding the socket and starting to listen
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) WS_die();
    if (listen(server_fd, 3) == SOCKET_ERROR) WS_die();

    // Starting mainloop to continuously accept connections and respond to them
    mainloop();
    WSACleanup();
}

char* getTemplate(char *path) {
    for (int i = 0; i < sizeof(routes) / sizeof(routes[0]); i++) {
        if (strcmp(routes[i].path, path) == 0) {
            return routes[i].template_name; // Loops through routes till expected route is found
        }
    }
    return NULL; // returns null otherwise
}

char* getPath(char *buffer) {
    // Request string is of the form "METHOD /PATH HTTPVERSION"
    char *path = strtok(buffer, " "); // splits request string by " "
    path = strtok(NULL, " "); // splits again to get the path
    return path;
}

void buildResponse(char *buffer, char *response) {
    char *path = getPath(buffer); // gets the path from the request string
    char *template = getTemplate(path); // gets the html file to be rendered

    if (template == NULL) {
        response = "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n<html><body>404 Not Found</body></html>"; // 404 Resposnse with appropriate status code
    } else {
        char html[10000] = "";
        getHTML(template, html); // reads the html file
        sprintf(response, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n%s", html); // builds response string from html file
    }
}

void mainloop() {
    for (;;) {
        connection = accept(server_fd, NULL, NULL); // accept connection
        if (connection == INVALID_SOCKET) WS_die();
        int valread = recv(connection, buffer, BUFFER_SIZE, 0); // read incoming request
        char response[10000] = "";
        buildResponse(buffer, response); // build response of appropriate html page
        send(connection, response, strlen(response), 0); // sends response along with Content-Length
        closesocket(connection); // Close the socket as this isnt a persistent connection
    }
}


void getHTML(char *path, char *html) { // nothing fancy, just read html file till EOF
    FILE *fp = fopen(path, "r");

    if (fp == NULL) {
        printf("Template not found %s", path);
        exit(EXIT_FAILURE);
    }

    char c = fgetc(fp);
    while (c != EOF) {
        html[strlen(html)] = c;
        c = fgetc(fp);
    }

    fclose(fp);
}

void WS_die() { // there are so many ways this could go wrong i just decided to extract it into a function lol
    fprintf(stderr, "WSAStartup failed with error: %d", WSAGetLastError());
    exit(EXIT_FAILURE);
}