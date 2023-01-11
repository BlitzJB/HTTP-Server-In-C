#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void WS_die();

struct Route {
    char *path;
    char *(*renderFunction)();
};

struct Server {
    int port;
    int buffer_size;
    WSADATA wsaData;
    char buffer[BUFFER_SIZE];
    SOCKET server_fd, connection;
    struct sockaddr_in address;
    struct Route routes[100];
    int numRoutes;
    void (*setup)(struct Server server);
    void (*startListen)(struct Server server);
    void (*addRoute)(struct Server server, char *path, char *(*renderFunction)());
};
 




void setup(struct Server server);
void startListen(struct Server server);
void addRoute(struct Server server, char *path, char *(*renderFunction)());

struct Server makeServer() {
    struct Server server;
    if (WSAStartup(MAKEWORD(2,2), &server.wsaData) != 0) WS_die("Winsock init");

    server.port = PORT;
    server.buffer_size = BUFFER_SIZE;
    server.server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server.server_fd == INVALID_SOCKET) WS_die("Creating socket");
    server.setup = setup;
    server.startListen = startListen;

    server.addRoute = addRoute;
    server.numRoutes = 0;

    server.address.sin_family = AF_INET; // Address family = Internet
    server.address.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP address
    server.address.sin_port = htons(server.port); // Port in network byte order (ie, converting it form an integer to address of the port)
    
    return server;
}

void startServer(struct Server server) {
    server.setup(server);
    server.startListen(server);
}

void setup(struct Server server) {
    if (bind(server.server_fd, (struct sockaddr *)&server.address, sizeof(server.address)) == SOCKET_ERROR) WS_die("Binding");
    if (listen(server.server_fd, 3) == SOCKET_ERROR) WS_die("Initiating Listening");
}

void startListen(struct Server server) {
    while (1) {
        server.connection = accept(server.server_fd, NULL, NULL);
        if (server.connection == INVALID_SOCKET) WS_die("Accepting request");
        int valread = recv(server.connection, server.buffer, server.buffer_size, 0);
        if (valread == SOCKET_ERROR) WS_die("Receiving request");

        printf("%s", server.buffer);
        send(server.connection, "HTTP/1.1 200 OK\nContent-Type: text/html\n\nHello World!", 53, 0);
        closesocket(server.connection);
    }
}


void WS_die(char* checkName) {
    printf("At %s, Winsock error: %d", checkName, WSAGetLastError());
    WSACleanup();
    exit(1);
}

#define NUM_STATUS_CODES 2
struct codeMsgMapStruct {
    int code;
    char* msg;
} codeMsgMap[NUM_STATUS_CODES] = {
    {200, "OK"},
    {404, "Not Found"}
};

char* getMsgForCode(int code) {
    for (int i = 0; i < NUM_STATUS_CODES -1; i++ ) {
        if (codeMsgMap[i].code == code) return codeMsgMap[i].msg;
    }
    return "OK";
};

char* makeResponse(int statusCode, char* contentType, char* body) {
    char *response = malloc(10000); 
    sprintf(response, "HTTP/1.1 %d %s\nContent-Type: %s\n\n%s", statusCode, getMsgForCode(statusCode), contentType, body);
};

char* readTemplate(char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("Error opening file");
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, file);
    fclose(file);

    string[fsize] = 0;

    return string;
};

/* WHAT_GOES_HERE? */ getRenderFunction(struct Server server, char* path) {
    for (int i=0; i < server.numRoutes; i++) {
        if (server.routes[i].path == path) {
            return server.routes[i].renderFunction;
        }
    }
    printf("Path %s Not Found", path);
}

void addRoute(struct Server server, char *path, char *(*renderFunction)()) {
    struct Route route;
    route.path = path;
    route.renderFunction = renderFunction;
    
    server.routes[++server.numRoutes] = route;
}