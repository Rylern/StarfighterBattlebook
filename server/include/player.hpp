#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include <SFML/Network.hpp>
#include "battlebook.hpp"
#include "team.hpp"



namespace Server {
    class Player
    {

    private:
        static const int NUMBER_OF_WARNINGS = 2;
        static const int INITIAL_HEALTH_POINTS = 12;

        Battlebook battlebook;
        Team team;
        sf::TcpSocket* client;
        int nbWarnings;
        bool inWarningMode;
        int HP;
        int choosenManeuver;
        int currentPage;

    public:
        Player();
        Player(sf::TcpSocket* client, Team team);
        ~Player();
        Battlebook* getBattlebook();
        bool isInWarningMode();
        Team getTeam();
        int getHP();
        sf::TcpSocket* getClient();
        void setClient(sf::TcpSocket* client);
        int getChoosenManeuver();
        void setChoosenManeuver(int maneuver);
        Page* getCurrentPage();
        void setCurrentPage(int page);
        void newTurn();
        std::vector<int> getPossibleHints(int maneuver);
        std::vector<int> getSurroundingsHints(int hint);
        bool isManoeuverValid(int maneuver);
        bool isManoeuverAlreadyIndicated();
        void updateHP(Battlebook* enemyBattlebook);
        void updateWarning(Battlebook* enemyBattlebook);
        bool keepPlaying();
        int determineNewPage(int maneuver);
    };
}

#endif