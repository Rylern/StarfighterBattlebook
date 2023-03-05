#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include "window.hpp"
#include "msgqueue.hpp"
#include "client.hpp"
#include "server.hpp"
#include "mainmenu.hpp"
#include "waitingforplayers.hpp"
#include "joingame.hpp"
#include "teamselection.hpp"
#include "game.hpp"
#include "endgame.hpp"
#include "soundmanager.hpp"


class UiController
{

public:
    UiController();
    ~UiController();
    void run();

private:
    SoundManager soundManager;
    MsgQueue<std::string> msgQueueClient;
    MsgQueue<std::string> msgQueueUi;
    Client* client;
    Server::Server* server;
    Ui::Window window;
    MainMenu mainMenu;
    WaitingForPlayers waitingForPlayers;
    JoinGame joinGame;
    TeamSelection teamSelection;
    Game game;
    EndGame endGame;

    void checkMsg();
    void handleEvent(const sf::Event& event);
    void runServer();
    void draw();
    void hideLayouts();
    void showWaitingForPlayers();
    void showTeamSelection();
    void showGame(Team team);
    void showEndGame(GameResult gameResult);
    void maneuverClicked(std::string maneuver);

    void hostGameClicked();
    void joinGameClicked();
    void quitClicked();
    void waitingForPlayersBackClicked();
    void joinGameBackClicked();
    void joinGameConnectClicked();
    void teamSelectionRebellionClicked();
    void teamSelectionEmpireClicked();
    void endGameRestartClicked();
    void endGameQuitClicked();
};

#endif