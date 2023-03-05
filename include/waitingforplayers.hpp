#ifndef WAITINGFORPLAYERS_H
#define WAITINGFORPLAYERS_H

#include "layout.hpp"
#include "button.hpp"
#include "label.hpp"
#include "msgqueue.hpp"


class WaitingForPlayers: public Ui::Layout {

public:
    WaitingForPlayers(MsgQueue<std::string>& msgQueue, Widget* parent = nullptr);
    ~WaitingForPlayers();
private:
    Ui::Label waiting;
    Ui::Button back;
    MsgQueue<std::string>* msgQueue;

    void backClicked();
};



#endif