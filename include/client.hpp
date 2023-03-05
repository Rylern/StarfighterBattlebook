#ifndef CLIENT_H
#define CLIENT_H

#include <SFML/Network.hpp>
#include <thread>
#include <future>
#include "msgqueue.hpp"

class Client {

public:
    Client(MsgQueue<std::string>& msgQueue, std::string ip);
    ~Client();
    void checkForMessages();
    void sendMessage(std::string message);
private:
    static const int PORT = 55001;
    MsgQueue<std::string>* msgQueue;
    sf::TcpSocket socket;
    std::thread thread;
};



#endif