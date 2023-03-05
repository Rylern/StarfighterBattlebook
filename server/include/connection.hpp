#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <list>
#include <SFML/Network.hpp>


namespace Server {
    enum ConnectionStatus {
        NOTHING,
        NEW_USER,
        MSG_FROM_CLIENT,
        STOP_SERVER,
        CLIENT_DISCONNECTED
    };

    class Connection
    {
    private:
        static const int PORT = 55001;
        
        bool connectionSet;
        std::list<sf::TcpSocket*> clients;
        sf::TcpListener listener;
        sf::SocketSelector selector;
        std::string lastMessageReceived;
        sf::TcpSocket* lastActiveClient;

        void clearClients();

    public:
        static const int MAX_NUMBER_CLIENTS = 2;

        Connection();
        ~Connection();
        bool isConnectionSet();
        ConnectionStatus getStatus();
        bool capacityFull();
        bool capacityEmpty();
        void writeToClient(sf::TcpSocket* client, std::string message);
        void writeToOtherClient(sf::TcpSocket* client, std::string message);
        void writeToAll(std::string message);
        std::string getLastMessageReceived();
        sf::TcpSocket* getLastActiveClient();
        sf::TcpSocket* getOtherClient();
    };
}

#endif