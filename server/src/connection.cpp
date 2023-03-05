#include "connection.hpp"
#include "exception.hpp"


namespace Server {
    Connection::Connection() {
        this->connectionSet = this->listener.listen(Connection::PORT) == sf::Socket::Status::Done;
        this->selector.add(listener);
    }

    Connection::~Connection() {
        this->clearClients();
        this->listener.close();
    }

    bool Connection::isConnectionSet() {
        return this->connectionSet;
    }

    ConnectionStatus Connection::getStatus() {
        ConnectionStatus connectionStatus = NOTHING;

        if (this->selector.wait(sf::seconds(0.1f))) {
            if (this->selector.isReady(this->listener)) {

                sf::TcpSocket* client = new sf::TcpSocket;
                if (this->listener.accept(*client) == sf::Socket::Done) {
                    if (this->clients.size() < Connection::MAX_NUMBER_CLIENTS) {
                        this->clients.push_back(client);
                        this->selector.add(*client);
                        connectionStatus = ConnectionStatus::NEW_USER;
                    } else {
                        writeToClient(client, "max_capacity_reached");
                        delete client;
                    }
                } else {
                    delete client;
                }
            } else {
                std::list<sf::TcpSocket*>::iterator it = this->clients.begin();
                while (it != this->clients.end()) {
                    sf::TcpSocket& client = **it;
                    if (selector.isReady(client)) {
                        this->lastActiveClient = &client;
                        sf::Packet packet;
                        sf::Socket::Status status = client.receive(packet);

                        switch (status) {
                            case sf::Socket::Disconnected:
                                selector.remove(client);
                                client.disconnect();
                                delete(&client);
                                this->clients.erase(it++);
                                connectionStatus = ConnectionStatus::CLIENT_DISCONNECTED;
                                break;
                            case sf::Socket::Done:
                                if (packet >> this->lastMessageReceived) {
                                    connectionStatus = ConnectionStatus::MSG_FROM_CLIENT;
                                } else {
                                    throw Exception("Message received error");
                                }
                                ++it;
                                break;
                            default:
                                break;
                        }
                    } else {
                        ++it;
                    }
                }
            }
        }

        return connectionStatus;
    }

    bool Connection::capacityFull() {
        return this->clients.size() >= Connection::MAX_NUMBER_CLIENTS;
    }

    bool Connection::capacityEmpty() {
        return this->clients.size() == 0;
    }

    void Connection::writeToClient(sf::TcpSocket* client, std::string message) {
        sf::Packet packet;
        packet << message;
        if (client->send(packet) != sf::Socket::Done) {
            throw Exception("Message write error");
        }
    }

    void Connection::writeToOtherClient(sf::TcpSocket* client, std::string message) {
        sf::TcpSocket* otherClient = nullptr;

        for (auto const& c : this->clients) {
            if (c != client) {
                otherClient = c;
            }
        }

        if (otherClient != nullptr) {
            this->writeToClient(otherClient, message);
        }
    }

    void Connection::writeToAll(std::string message) {
        for (auto const& client : this->clients) {
            this->writeToClient(client, message);
        }
    }

    std::string Connection::getLastMessageReceived() {
        return this->lastMessageReceived;
    }

    sf::TcpSocket* Connection::getLastActiveClient() {
        return this->lastActiveClient;
    }

    sf::TcpSocket* Connection::getOtherClient() {
        sf::TcpSocket* otherClient = nullptr;

        for (auto const& client : this->clients) {
            if (client != this->lastActiveClient) {
                otherClient = client; 
            }
        }

        return otherClient;
    }

    void Connection::clearClients() {
        std::list<sf::TcpSocket*>::iterator it = this->clients.begin();
        
        while (it != this->clients.end()) {
            sf::TcpSocket& client = **it;
            this->selector.remove(client);
            client.disconnect();
            delete(&client);
            this->clients.erase(it++);
        }
    }
}