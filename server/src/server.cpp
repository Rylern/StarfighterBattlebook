#include "server.hpp"
#include "exception.hpp"
#include <iostream>

namespace Server {
    Server::Server() {
        this->game = nullptr;
        this->step = ServerStatus::WAITING_FOR_PLAYERS;

        this->running = this->connection.isConnectionSet();
    }

    Server::~Server() {
        if (this->game != nullptr) {
            delete this->game;
        }
    }

    bool Server::isRunning() {
        return this->running;
    }

    void Server::run() {
        ConnectionStatus status = this->connection.getStatus();

        switch (status) {
            case ConnectionStatus::NEW_USER:
                this->newUser();
                break;
            case ConnectionStatus::MSG_FROM_CLIENT:
                this->messageFromClient();
                break;
            case ConnectionStatus::STOP_SERVER:
                this->running = false;
                break;
            case ConnectionStatus::CLIENT_DISCONNECTED:
                this->clientDisconnected();
                break;
            default:
                break;
        }
    }


    void Server::newUser() {
        switch (this->step) {
            case ServerStatus::WAITING_FOR_PLAYERS:
                this->addNewPlayer();
                break;
            case ServerStatus::GAME_PAUSED:
                this->playerCameBack();
                break;
            case ServerStatus::WAITING_FOR_RESTART:
                this->teamSelection();
                break;
            default:
                break;
        }
    }

    void Server::addNewPlayer() {
        if (this->connection.capacityFull()) {
            this->teamSelection();
        } else {
            this->connection.writeToAll("waiting_for_players");
            this->step = ServerStatus::WAITING_FOR_PLAYERS;
        }
    }

    void Server::playerCameBack() {
        this->step = ServerStatus::MANEUVER_CHOICE;
        this->game->clientCameBack(this->connection.getLastActiveClient());
        this->game->newTurn();
    }

    void Server::teamSelection() {
        this->connection.writeToAll("team_selection");
        this->step = ServerStatus::TEAM_CHOICE;
    }

    void Server::messageFromClient() {
        switch (this->step) {
            case ServerStatus::TEAM_CHOICE:
                this->teamChoice();
                break;
            case ServerStatus::MANEUVER_CHOICE:
                this->manoeuverChoice();
                break;
            case ServerStatus::WAITING_FOR_RESTART:
                this->restartResponse();
                break;
            default:
                break;
        }
    }

    void Server::teamChoice() {
        int team = std::stoi(this->connection.getLastMessageReceived());

        if (team == Team::REBELLION || team == Team::EMPIRE) {
            this->step = ServerStatus::MANEUVER_CHOICE;

            try {
                this->game = new Game(&(this->connection), (Team) team);
                this->game->newTurn();
            } catch (const Exception& exception) {
                this->running = false;
                std::cerr << exception.getMessage() << std::endl;
            }
        }
    }

    void Server::manoeuverChoice() {
        int maneuver = std::stoi(this->connection.getLastMessageReceived());

        try {
            bool continueGame = this->game->chooseManeuverAndContinueGame(maneuver, this->connection.getLastActiveClient());
            if (!continueGame) {
                this->step = ServerStatus::WAITING_FOR_RESTART;
            }
        } catch (const Exception& exception) {
            this->running = false;
            std::cerr << exception.getMessage() << std::endl;
        }
    }

    void Server::restartResponse() {
        if (this->connection.getLastMessageReceived() == "new_game") {
            this->addNewPlayer();
        }
    }

    void Server::clientDisconnected() {
        if (this->connection.capacityEmpty()) {
            this->step = ServerStatus::WAITING_FOR_PLAYERS;
        } else {
            switch (this->step) {
                case ServerStatus::WAITING_FOR_RESTART:
                case ServerStatus::TEAM_CHOICE:
                    this->connection.writeToAll("other_player_disconnected");
                    this->step = ServerStatus::WAITING_FOR_PLAYERS;
                    break;
                case ServerStatus::MANEUVER_CHOICE:
                    this->game->removeClient(this->connection.getLastActiveClient());
                    this->connection.writeToAll("other_player_disconnected");
                    this->step = ServerStatus::GAME_PAUSED;
                    break;
                default:
                    break;
            }
        }
    }
}