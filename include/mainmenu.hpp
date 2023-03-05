#ifndef MAINMENU_H
#define MAINMENU_H

#include "layout.hpp"
#include "button.hpp"
#include "msgqueue.hpp"

class MainMenu: public Ui::Layout {

public:
    MainMenu(MsgQueue<std::string>& msgQueue, Widget* parent = nullptr);
    ~MainMenu();
private:
    Ui::Button hostGame;
    Ui::Button joinGame;
    Ui::Button quit;
    MsgQueue<std::string>* msgQueue;

    void hostGameClicked();
    void joinGameClicked();
    void quitClicked();
};



#endif