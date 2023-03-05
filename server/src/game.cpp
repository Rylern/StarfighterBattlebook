#include "game.hpp"
#include "exception.hpp"


namespace Server {
	Game::Game() {
		this->playerInWarning = nullptr;
		this->hintGiven = false;
		this->nbOfPlayersReady = 0;
	}

	Game::Game(Connection* connection, Team team): Game() {
		this->connection = connection;

		if (team == Team::REBELLION) {
			this->connection->writeToClient(this->connection->getLastActiveClient(), "rebellion");
			this->players[0] = new Player(this->connection->getLastActiveClient(), Team::REBELLION);

			this->connection->writeToClient(this->connection->getOtherClient(), "empire");
			this->players[1] = new Player(this->connection->getOtherClient(), Team::EMPIRE);
		} else {
			this->connection->writeToClient(this->connection->getLastActiveClient(), "empire");
			this->players[0] = new Player(this->connection->getLastActiveClient(), Team::EMPIRE);

			this->connection->writeToClient(this->connection->getOtherClient(), "rebellion");
			this->players[1] = new Player(this->connection->getOtherClient(), Team::REBELLION);
		}
	}

	Game::~Game() {
		if (this->players[0] != nullptr) {
			delete this->players[0];
		}
		
		if (this->players[1] != nullptr) {
			delete this->players[1];
		}
	}

	void Game::newTurn() {
		for (Player* player: this->players) {
			player->newTurn();
		}

		this->setTeamInWarning();
		this->sendNewTurnInformation();

		this->hintGiven = false;
		this->nbOfPlayersReady = 0;
	}

	bool Game::chooseManeuverAndContinueGame(int maneuver, sf::TcpSocket* client) {
		bool continueGame = true;

		Player* player = nullptr;
		for (Player* p: this->players) {
			if (p->getClient() == client) {
				player = p;
			}
		}
		if (player == nullptr) {
			throw Exception("Player null");
		}

		if (this->isMessageHint(player)) {
			this->hintGiven = this->isHintAccepted(player, maneuver);
			if (this->hintGiven) {
				this->connection->writeToClient(player->getClient(), "hint_accepted");

				std::vector<int> surroundingsHints = player->getSurroundingsHints(maneuver);

				std::string message = "get_hints:";
				for (int hint: surroundingsHints) {
					message += std::to_string(hint) + "-";
				}
				message.pop_back();
				this->connection->writeToOtherClient(player->getClient(), message);
			} else {
				this->connection->writeToClient(player->getClient(), "error:invalid_hint");
			}
		} else {
			if (isManoeuverValid(maneuver, player)) {
				this->addManeuver(maneuver, player);
				this->nbOfPlayersReady++;

				if (this->nbOfPlayersReady >= Connection::MAX_NUMBER_CLIENTS) {
					try {
						this->determineNewPages();
						this->endOfTurn();

						if (this->keepPlaying()) {
							this->newTurn();
						} else {
							continueGame = false;
						}
					} catch (const Exception&) {
						this->newTurn();	
					}
				}
			}
		}
		return continueGame;
	}

	void Game::removeClient(sf::TcpSocket* client) {
		for (Player* player: this->players) {
			if (player->getClient() == client) {
				player->setClient(nullptr);
			}
		}
	}

	void Game::clientCameBack(sf::TcpSocket* client) {
		for (Player* player: this->players) {
			if (player->getClient() == nullptr) {
				player->setClient(client);
			}

			if (player->getTeam() == Team::REBELLION) {
				this->connection->writeToClient(player->getClient(), "rebellion");
			} else {
				this->connection->writeToClient(player->getClient(), "empire");
			}
		}
	}

	void Game::setTeamInWarning() {
		this->playerInWarning = nullptr;

		for (Player* player: this->players) {
			if (player->isInWarningMode()) {
				if (this->playerInWarning == nullptr) {
					this->playerInWarning = player;
				} else {
					this->playerInWarning = nullptr;
				}
			}
		}
	}

	void Game::sendNewTurnInformation() {
		for (Player* player: this->players) {
			std::string warning;
			if (this->playerInWarning == nullptr) {
				warning = "0";
			} else if (this->playerInWarning == player) {
				warning = "1";
			} else {
				warning = "2";
			}

			std::string message = "new_turn:";
			message += std::to_string(player->getCurrentPage()->number) + "-";
			message += std::to_string(player->getCurrentPage()->damageDealt) + "-";
			message += std::to_string(player->getCurrentPage()->damageTaken) + "-";
			message += std::to_string(player->getCurrentPage()->warning) + "-";
			message += std::to_string(player->getCurrentPage()->lockOn) + "-";
			message += std::to_string(player->getCurrentPage()->view) + "-";
			message += std::to_string(player->getCurrentPage()->range) + "-";
			message += std::to_string(player->getHP()) + "-";
			message += warning;

			this->connection->writeToClient(player->getClient(), message);
		}
	}

	bool Game::isMessageHint(Player* player) {
		return this->playerInWarning == player && !this->hintGiven && player->getChoosenManeuver() != -1;
	}

	bool Game::isHintAccepted(Player* player, int hint) {
		bool hintAccepted = false;

		std::vector<int> possiblesHints = player->getPossibleHints(player->getChoosenManeuver());
		for (int possiblesHint: possiblesHints) {
			hintAccepted = hintAccepted || possiblesHint == hint;
		}

		return hintAccepted;
	}

	bool Game::isManoeuverValid(int maneuver, Player* player) {
		bool maneuverValid;

		if (player->isManoeuverValid(maneuver)) {
			if (player->isManoeuverAlreadyIndicated()) {
				maneuverValid = false;
				this->connection->writeToClient(player->getClient(), "error:maneuver_already_indicated");
			} else {
				maneuverValid = true;
			}
		} else {
			maneuverValid = false;
			this->connection->writeToClient(player->getClient(), "error:invalid_maneuver");
		}

		return maneuverValid;
	}

	void Game::addManeuver(int maneuver, Player* player) {
		if (this->playerInWarning == player && player->getChoosenManeuver() == -1) {
			this->sendChooseHintMessage(maneuver, player);
		} else {
			this->connection->writeToClient(player->getClient(), "maneuver_accepted");
		}

		player->setChoosenManeuver(maneuver);
	}

	void Game::sendChooseHintMessage(int maneuver, Player* player) {
		std::vector<int> possiblesHints = player->getPossibleHints(maneuver);

		std::string message = "choose_hint:";
		for (int possiblesHint: possiblesHints) {
			message += std::to_string(possiblesHint) + "-";
		}
		message.pop_back();

		this->connection->writeToClient(player->getClient(), message);
	}

	void Game::determineNewPages() {
		int maneuver0 = this->players[0]->getChoosenManeuver();
		int maneuver1 = this->players[1]->getChoosenManeuver();

		int midPage0 = this->players[1]->determineNewPage(maneuver1);
		int midPage1 = this->players[0]->determineNewPage(maneuver0);

		int endPage0;
		int endPage1;
		if ((midPage0 == Battlebook::NUMBER_OF_PAGES) && (midPage1 == Battlebook::NUMBER_OF_PAGES)) {
			endPage1 = Battlebook::NUMBER_OF_PAGES;
			endPage0 = Battlebook::NUMBER_OF_PAGES;
		}
		else if (midPage1 == Battlebook::NUMBER_OF_PAGES) {
			endPage0 = this->players[0]->getBattlebook()->determineNewPage(midPage0, maneuver0);
			endPage1 = endPage0;
		}
		else if (midPage0 == Battlebook::NUMBER_OF_PAGES) {
			endPage1 = this->players[1]->getBattlebook()->determineNewPage(midPage1, maneuver1);
			endPage0 = endPage1;
		}
		else {
			endPage0 = this->players[0]->getBattlebook()->determineNewPage(midPage0, maneuver0);
			endPage1 = this->players[1]->getBattlebook()->determineNewPage(midPage1, maneuver1);
		}

		if (endPage0 != endPage1) {
			std::string errorMessage = "Final pages do not match (error in file):\n";
			errorMessage += "Begin Page 0: " + std::to_string(this->players[0]->getCurrentPage()->number) + "\n";
			errorMessage += "Begin Page 1: " + std::to_string(this->players[1]->getCurrentPage()->number) + "\n";
			errorMessage += "Maneuver 0: " + std::to_string(maneuver0) + "\n";
			errorMessage += "Maneuver 1: " + std::to_string(maneuver1) + "\n";
			errorMessage += "Mid Page 0: " + std::to_string(midPage0) + "\n";
			errorMessage += "Mid Page 1: " + std::to_string(midPage1) + "\n";
			errorMessage += "End Page 0: " + std::to_string(endPage0) + "\n";
			errorMessage += "End Page 1: " + std::to_string(endPage1) + "\n";
			throw Exception(errorMessage);
		} else {
			for (Player* player: this->players) {
				player->setCurrentPage(endPage0);
			}
		}
	}

	void Game::endOfTurn() {
		this->players[0]->updateHP(this->players[1]->getBattlebook());
		this->players[1]->updateHP(this->players[0]->getBattlebook()); 
		this->players[0]->updateWarning(this->players[1]->getBattlebook());
		this->players[1]->updateWarning(this->players[0]->getBattlebook());
	}

	bool Game::keepPlaying() {
		bool status = true;

		if (this->players[0]->getHP() == 0) {
			this->connection->writeToClient(this->players[0]->getClient(), "defeat");
			this->connection->writeToClient(this->players[1]->getClient(), "victory");
			status = false;
		} else if (this->players[1]->getHP() == 0) {
			this->connection->writeToClient(this->players[1]->getClient(), "defeat");
			this->connection->writeToClient(this->players[0]->getClient(), "victory");
			status = false;
		} else if (!this->players[0]->keepPlaying()) {
			this->connection->writeToAll("draw");
			status = false;
		}

		return status;
	}

	void Game::setPlayers(std::array<Player*, Connection::MAX_NUMBER_CLIENTS> newPlayers) {
		this->players = newPlayers;
	}
}