#include "client.hpp"

Client::Client(MsgQueue<std::string>& msgQueue, std::string ip) {
    this->msgQueue = &msgQueue;
    
    this->thread = std::thread([this, ip]() {
        sf::Socket::Status status = this->socket.connect(ip, this->PORT, sf::seconds(2));

        this->socket.setBlocking(false);
        if (status == sf::Socket::Done) {
            this->msgQueue->send("connection_success");
        } else {
            this->msgQueue->send("connection_error");
        }
    });
}

Client::~Client() {
    this->socket.disconnect();

    this->thread.join();
}

void Client::checkForMessages() {
    sf::Packet packet;
    std::string message;

    sf::Socket::Status status = this->socket.receive(packet);
    switch (status) {
        case sf::Socket::Disconnected:
            this->msgQueue->send("connection_error");
            break;
        case sf::Socket::Done:
            if (packet >> message) {
                this->msgQueue->send(std::move(message));
            } else {
                this->msgQueue->send("read_message_error");
            }
            break;
        default:
            break;
    }
}

void Client::sendMessage(std::string message) {
    sf::Packet packet;
	packet << message;

    this->socket.setBlocking(true);
	if (this->socket.send(packet) != sf::Socket::Done) {
        this->msgQueue->send("send_message_error");
	}
    this->socket.setBlocking(false);
}