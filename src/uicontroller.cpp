#include "uicontroller.hpp"
#include "utils.hpp"

UiController::UiController():   client(nullptr),
                                server(nullptr),
                                window(sf::VideoMode(800, 600), "Starfighter Battlebook"),
                                mainMenu(this->msgQueueUi, &(this->window)),
                                waitingForPlayers(this->msgQueueUi, &(this->window)),
                                joinGame(this->msgQueueUi, &(this->window)),
                                teamSelection(this->msgQueueUi, &(this->window)),
                                game(this->soundManager, this->msgQueueUi, &(this->window)),
                                endGame(this->msgQueueUi, &(this->window)) {
    this->window.setImage(Utils::joinPath({"res", "game.png"}));

    sf::Image icon;
    if (icon.loadFromFile(Utils::joinPath({"res", "icon.png"}))) {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    this->soundManager.playMenuMusic();
}

UiController::~UiController() {
    if (this->client != nullptr) {
        delete this->client;
    }
    if (this->server != nullptr) {
        delete this->server;
    }
}

void UiController::run() {
    sf::Event event;
    
    while (this->window.isOpen()) {
        this->checkMsg();

        this->runServer();

        while (this->window.pollEvent(event)) {
            this->handleEvent(event);
        }

        this->window.clear(sf::Color::White);
        this->draw();
        this->window.display();
    }
}

void UiController::checkMsg() {
    if (this->client != nullptr) {
        this->client->checkForMessages();

        while (this->msgQueueClient.messageAvailable()) {
            std::string message = this->msgQueueClient.receive();

            if (message == "connection_success") {
                this->joinGame.setStatus("Success");
            } else if (message == "connection_error") {
                delete this->client;
                this->client = nullptr;
                if (this->server != nullptr) {
                    delete this->server;
                    this->server = nullptr;
                }
                this->joinGameClicked();
                this->joinGame.setStatus("Connection error");
            } else if (message == "waiting_for_players") {
                this->showWaitingForPlayers();
            } else if (message == "team_selection") {
                this->showTeamSelection();
            } else if (message == "rebellion") {
                this->showGame(Team::REBELLION);
            } else if (message == "empire") {
                this->showGame(Team::EMPIRE);
            } else if (message.find("new_turn") != std::string::npos) {
                this->game.newTurn(message.substr(9));
            } else if (message == "maneuver_accepted") {
                this->game.maneuverAccepted();
            } else if (message.find("choose_hint") != std::string::npos) {
                this->game.chooseHint(message.substr(12));
            } else if (message == "hint_accepted") {
                this->game.hintAccepted();
            } else if (message == "error:invalid_hint") {
                this->game.hintNotAccepted();
            } else if (message.find("get_hints") != std::string::npos) {
                this->game.seeHints(message.substr(10));
            } else if (message == "draw") {
                this->showEndGame(GameResult::DRAW);
            } else if (message == "defeat") {
                this->showEndGame(GameResult::LOSE);
            } else if (message == "victory") {
                this->showEndGame(GameResult::WIN);
            } else if (message == "other_player_disconnected") {
                this->soundManager.playMenuMusic();
                this->showWaitingForPlayers();
            }
        }
    }

    while (this->msgQueueUi.messageAvailable()) {
        std::string message = this->msgQueueUi.receive();
        if (message == "hostGameClicked") {
            this->hostGameClicked();
        } else if (message == "joinGameClicked") {
            this->joinGameClicked();
        } else if (message == "quitClicked") {
            this->quitClicked();
        } else if (message == "waitingForPlayersBackClicked") {
            this->waitingForPlayersBackClicked();
        } else if (message == "joinGameBackClicked") {
            this->joinGameBackClicked();
        } else if (message == "joinGameConnectClicked") {
            this->joinGameConnectClicked();
        } else if (message == "teamSelectionRebellionClicked") {
            this->teamSelectionRebellionClicked();
        } else if (message == "teamSelectionEmpireClicked") {
            this->teamSelectionEmpireClicked();
        } else if (message.find("maneuver") != std::string::npos) {
            this->maneuverClicked(message.substr(9));
        } else if (message == "endGameRestartClicked") {
            this->endGameRestartClicked();
        } else if (message == "endGameQuitClicked") {
            this->endGameQuitClicked();
        }
    }
}

void UiController::runServer() {
    if (this->server != nullptr) {
        if (this->server->isRunning()) {
            this->server->run();
        } else {
            delete this->server;
            this->server = nullptr;
        }
    }
}

void UiController::handleEvent(const sf::Event& event) {
    this->soundManager.handleEvent();
    this->window.handleEvent(event);
    this->mainMenu.handleEvent(event);
    this->waitingForPlayers.handleEvent(event);
    this->joinGame.handleEvent(event);
    this->teamSelection.handleEvent(event);
    this->game.handleEvent(event);
    this->endGame.handleEvent(event);

    if (event.type == sf::Event::Closed) {
        this->window.close();
    }
}

void UiController::draw() {
    this->window.draw(this->window);
    this->window.draw(this->mainMenu);
    this->window.draw(this->waitingForPlayers);
    this->window.draw(this->joinGame);
    this->window.draw(this->teamSelection);
    this->window.draw(this->game);
    this->window.draw(this->endGame);
}

void UiController::hideLayouts() {
    this->mainMenu.hide();
    this->waitingForPlayers.hide();
    this->joinGame.hide();
    this->teamSelection.hide();
    this->game.hide();
    this->endGame.hide();
    this->window.showImage();
}

void UiController::showWaitingForPlayers() {
    this->hideLayouts();
    this->waitingForPlayers.show();
}

void UiController::showTeamSelection() {
    this->hideLayouts();
    this->teamSelection.show();
}

void UiController::showGame(Team team) {
    this->soundManager.pauseMenuMusic();

    this->hideLayouts();
    this->window.hideImage();
    this->game.setTeam(team);
    this->game.show();

    this->soundManager.fightersComing(this->game.getTeam());
}

void UiController::showEndGame(GameResult gameResult) {
    this->hideLayouts();
    this->window.hideImage();
    this->endGame.setResult(gameResult, this->game.getTeam());
    this->endGame.show();

    if (gameResult == GameResult::WIN) {
        this->soundManager.lasersAndExplosion(this->game.getTeam());
    } else if (gameResult == GameResult::LOSE) {
        this->soundManager.explosion(this->game.getTeam());
    }
}

void UiController::hostGameClicked() {
    this->hideLayouts();
    this->showWaitingForPlayers();

    this->server = new Server::Server();
    this->client = new Client(this->msgQueueClient, "127.0.0.1");
}

void UiController::joinGameClicked() {
    this->hideLayouts();
    this->joinGame.resetInput();
    this->joinGame.show();
}

void UiController::quitClicked() {
    this->window.close();
}

void UiController::waitingForPlayersBackClicked() {
    delete this->client;
    this->client = nullptr;
    if (this->server != nullptr) {
        delete this->server;
        this->server = nullptr;
    }
    this->hideLayouts();
    this->mainMenu.show();
}

void UiController::joinGameConnectClicked() {
    std::string input = this->joinGame.getIP();
    if (input != "") {
        this->joinGame.setStatus("Connecting...");
        this->client = new Client(this->msgQueueClient, input);
    }
}

void UiController::joinGameBackClicked() {
    this->hideLayouts();
    this->mainMenu.show();
}

void UiController::teamSelectionRebellionClicked() {
    this->client->sendMessage("1");
}

void UiController::teamSelectionEmpireClicked() {
    this->client->sendMessage("2");
}

void UiController::maneuverClicked(std::string maneuver) {
    this->client->sendMessage(maneuver);
}

void UiController::endGameRestartClicked() {
    this->client->sendMessage("new_game");
}

void UiController::endGameQuitClicked() {
    delete this->client;
    this->client = nullptr;
    if (this->server != nullptr) {
        delete this->server;
        this->server = nullptr;
    }

    this->joinGameClicked();
    this->soundManager.playMenuMusic();
}