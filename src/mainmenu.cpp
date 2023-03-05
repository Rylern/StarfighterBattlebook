#include "mainmenu.hpp"



MainMenu::MainMenu(MsgQueue<std::string>& msgQueue, Ui::Widget* parent):    Ui::Layout(parent),
                                                                            hostGame(this),
                                                                            joinGame(this),
                                                                            quit(this) {
    this->msgQueue = &msgQueue;

    this->hostGame.setText("Host game");
    this->hostGame.setOnClick<MainMenu>(this, &MainMenu::hostGameClicked);

    this->joinGame.setText("Join game");
    this->joinGame.setOnClick<MainMenu>(this, &MainMenu::joinGameClicked);

    this->quit.setText("Quit");
    this->quit.setOnClick<MainMenu>(this, &MainMenu::quitClicked);

    this->setPositionRelativeY(0.18f);
    this->setChildMargin(20);
    this->add(this->hostGame);
    this->add(this->joinGame);
    this->add(this->quit);
}

MainMenu::~MainMenu() {

}

void MainMenu::hostGameClicked() {
    this->msgQueue->send("hostGameClicked");
}

void MainMenu::joinGameClicked() {
    this->msgQueue->send("joinGameClicked");
}

void MainMenu::quitClicked() {
    this->msgQueue->send("quitClicked");
}