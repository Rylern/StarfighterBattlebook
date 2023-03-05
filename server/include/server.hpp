#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "connection.hpp"
#include "game.hpp"

namespace Server {
    enum ServerStatus {
        WAITING_FOR_PLAYERS,
        TEAM_CHOICE,
        GAME_PAUSED,
        MANEUVER_CHOICE,
        WAITING_FOR_RESTART
    };


    class Server
    {
    private:
        Connection connection;
        ServerStatus step;
        Game* game;
        bool running;

        void newUser();
        void addNewPlayer();
        void playerCameBack();
        void teamSelection();
        void messageFromClient();
        void teamChoice();
        void manoeuverChoice();
        void restartResponse();
        void clientDisconnected();

    public:
        Server();
        ~Server();
        bool isRunning();
        void run();

    };
}

#endif