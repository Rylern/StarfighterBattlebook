#include "server.hpp"
#include <csignal>
#include <iostream>


Server::Server* server;

void signalHandler(int signum) {
    delete(server);
    exit(signum);  
}


int main() {
    std::signal(SIGINT, signalHandler);  

    server = new Server::Server();
    std::cout << "Server started" << std::endl;
    while (server->isRunning()) {
        server->run();
    }
    delete(server);
    std::cout << "Server closed" << std::endl;

    return 0;
}