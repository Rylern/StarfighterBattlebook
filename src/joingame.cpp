#include "joingame.hpp"



JoinGame::JoinGame(MsgQueue<std::string>& msgQueue, Ui::Widget* parent):    Ui::Layout(parent),
                                                                            IP(this),
                                                                            connect(this),
                                                                            status(this),
                                                                            back(this),
                                                                            lastIP("") {
    this->msgQueue = &msgQueue;

    this->IP.setText("IP:");
    this->IP.setMaxInput("999.999.999.999");

    this->connect.setText("Connect");
    this->connect.setOnClick<JoinGame>(this, &JoinGame::connectClicked);

    this->back.setText("Back");
    this->back.setOnClick<JoinGame>(this, &JoinGame::backClicked);

    this->setPositionRelativeY(0.2f);
    this->setChildMargin(15);
    this->add(this->IP);
    this->add(this->connect);
    this->add(this->status);
    this->add(this->back);
    this->hide();
}

JoinGame::~JoinGame() {

}

void JoinGame::setStatus(std::string newStatus) {
    this->status.setText(newStatus, 36);
}

void JoinGame::resetInput() {
    this->setStatus("");
    this->IP.setInput(this->lastIP);
}

std::string JoinGame::getIP() {
    this->lastIP = this->IP.getInput();
    return this->IP.getInput();
}

void JoinGame::backClicked() {
    this->msgQueue->send("joinGameBackClicked");
}

void JoinGame::connectClicked() {
    this->msgQueue->send("joinGameConnectClicked");
}