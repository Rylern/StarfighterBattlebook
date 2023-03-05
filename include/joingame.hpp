#ifndef JOINGAME_H
#define JOINGAME_H

#include "layout.hpp"
#include "button.hpp"
#include "label.hpp"
#include "textfield.hpp"
#include "msgqueue.hpp"

class JoinGame: public Ui::Layout {

public:
    JoinGame(MsgQueue<std::string>& msgQueue, Widget* parent = nullptr);
    ~JoinGame();
    void setStatus(std::string status);
    void resetInput();
    std::string getIP();
private:
    Ui::TextField IP;
    Ui::Button connect;
    Ui::Label status;
    Ui::Button back;
    MsgQueue<std::string>* msgQueue;
    std::string lastIP;

    void backClicked();
    void connectClicked();
};



#endif