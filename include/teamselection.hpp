#ifndef TEAMSELECTION_H
#define TEAMSELECTION_H

#include "layout.hpp"
#include "button.hpp"
#include "msgqueue.hpp"

class TeamSelection: public Ui::Layout {

public:
    TeamSelection(MsgQueue<std::string>& msgQueue, Widget* parent = nullptr);
    ~TeamSelection();
    void handleEvent(const sf::Event& event);
private:
    Ui::Button rebellion;
    Ui::Button empire;
    MsgQueue<std::string>* msgQueue;

    void rebellionClicked();
    void empireClicked();
};



#endif