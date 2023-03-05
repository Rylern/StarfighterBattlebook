#include "player.hpp"
#include "exception.hpp"


namespace Server {
    Player::Player() {}

    Player::Player(sf::TcpSocket* client, Team team): battlebook(team) {
        this->client = client;
        this->team = team;
        this->nbWarnings = 0;
        this->inWarningMode = false;
        this->HP = INITIAL_HEALTH_POINTS;
        this->currentPage = Battlebook::BEGINNING_PAGE;
    }

    Player::~Player() {}

    Battlebook* Player::getBattlebook() {
        return &(this->battlebook);
    }

    bool Player::isInWarningMode() {
        return this->inWarningMode;
    }

    Team Player::getTeam() {
        return this->team;
    }

    int Player::getHP() {
        return this->HP;
    }

    sf::TcpSocket* Player::getClient() {
        return this->client;
    }

    void Player::setClient(sf::TcpSocket* newClient) {
        this->client = newClient;
    }

    int Player::getChoosenManeuver() {
        return this->choosenManeuver;
    }

    void Player::setChoosenManeuver(int maneuver) {
        this->choosenManeuver = maneuver;
    }

    Page* Player::getCurrentPage() {
        return this->battlebook.getPage(this->currentPage);
    }

    void Player::setCurrentPage(int page) {
        this->currentPage = page;
    }

    void Player::newTurn() {
        this->inWarningMode = this->nbWarnings > this->NUMBER_OF_WARNINGS;
        this->choosenManeuver = -1;
    }

    std::vector<int> Player::getPossibleHints(int maneuver) {
        return this->battlebook.getPossibleHints(maneuver);
    }

    std::vector<int> Player::getSurroundingsHints(int hint) {
        return this->battlebook.getSurroundingsHints(hint);
    }

    bool Player::isManoeuverValid(int maneuver) {
        return maneuver >= 0 && maneuver < this->battlebook.getNbManeuvers();
    }

    bool Player::isManoeuverAlreadyIndicated() {
        return this->choosenManeuver != -1;
    }

    void Player::updateHP(Battlebook* enemyBattlebook) {
        if (this->keepPlaying()) {
            int damage = enemyBattlebook->getPage(this->currentPage)->damageDealt;
            if (damage != this->getCurrentPage()->damageTaken) {
                std::string errorMessage = "Damage dealt and taken do not match (error in file):\n";
                errorMessage += "Page 0: " + std::to_string(this->getCurrentPage()->number) + "\n";
                errorMessage += "Page 1: " + std::to_string(enemyBattlebook->getPage(this->currentPage)->number) + "\n";
                throw Exception(errorMessage);
            }

            if ((this->HP - damage) >= 0) {
                this->HP = this->HP - damage;
            } else {
                this->HP = 0;
            }
        }
    }

    void Player::updateWarning(Battlebook* enemyBattlebook) {
        if (this->keepPlaying()) {
            if (this->getCurrentPage()->range == 0) {
            this->nbWarnings = 0;
            } else {
                int warning = this->getCurrentPage()->warning;
                if (warning != enemyBattlebook->getPage(this->currentPage)->lockOn) {
                    std::string errorMessage = "Warning and lock-on do not match (error in file):\n";
                    errorMessage += "Page 0: " + std::to_string(this->getCurrentPage()->number) + "\n";
                    errorMessage += "Page 1: " + std::to_string(enemyBattlebook->getPage(this->currentPage)->number) + "\n";
                    throw Exception(errorMessage);
                }
                
                if (warning != 0 && enemyBattlebook->getPage(this->currentPage)->warning == 0) {
                    this->nbWarnings++;
                } else {
                    this->nbWarnings = 0;
                }
            }
        }
    }

    bool Player::keepPlaying() {
        return this->currentPage != Battlebook::NUMBER_OF_PAGES;
    }

    int Player::determineNewPage(int maneuver) {
        return this->getBattlebook()->determineNewPage(this->currentPage, maneuver);
    }
}
