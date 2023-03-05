#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <array>

#include "battlebook.hpp"
#include "connection.hpp"
#include "player.hpp"



namespace Server {
    class Game
    {

    private:
        Connection* connection;
        std::array<Player*, Connection::MAX_NUMBER_CLIENTS> players;
        Player* playerInWarning;
        bool hintGiven;
        int nbOfPlayersReady;

        void setTeamInWarning();
        void sendNewTurnInformation();
        bool isMessageHint(Player* player);
        bool isHintAccepted(Player* player, int hint);
        bool isManoeuverValid(int maneuver, Player* player);
        void addManeuver(int maneuver, Player* player);
        void sendChooseHintMessage(int maneuver, Player* player);
        bool keepPlaying();

    public:
        Game();
        Game(Connection* connection, Team team);
        ~Game();
        void newTurn();
        bool chooseManeuverAndContinueGame(int maneuver, sf::TcpSocket* client);
        void removeClient(sf::TcpSocket* client);
        void clientCameBack(sf::TcpSocket* client);
        void determineNewPages();
        void endOfTurn();
        void setPlayers(std::array<Player*, Connection::MAX_NUMBER_CLIENTS> players);
    };
}

#endif