#include <stdio.h>
#include <stdlib.h>

#include "server.c"

char* renderIndex();
char* renderAbout();


int main() {
    struct Server server = makeServer();

    addRoute(server, "/index", renderIndex);
    addRoute(server, "/about", renderAbout);
    startServer(server);
}

char* renderIndex() {
    return readFile("index.html");
};

char* renderAbout() {
    return readFile("about.html");
};