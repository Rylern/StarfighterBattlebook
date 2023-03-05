#include "waitingforplayers.hpp"



WaitingForPlayers::WaitingForPlayers(MsgQueue<std::string>& msgQueue, Ui::Widget* parent):  Ui::Layout(parent),
                                                                                            waiting(this),
                                                                                            back(this) {
    this->msgQueue = &msgQueue;

    this->waiting.setText("Waiting for players...");

    this->back.setText("Back");
    this->back.setOnClick<WaitingForPlayers>(this, &WaitingForPlayers::backClicked);

    this->setPositionRelativeY(0.2f);
    this->setChildMargin(60);
    this->add(this->waiting);
    this->add(this->back);
    this->hide();
}

WaitingForPlayers::~WaitingForPlayers() {

}

void WaitingForPlayers::backClicked() {
    this->msgQueue->send("waitingForPlayersBackClicked");
}