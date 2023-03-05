#include "endgame.hpp"
#include "utils.hpp"


EndGame::EndGame(MsgQueue<std::string>& msgQueue, Ui::Widget* parent):  Ui::Layout(parent),
                                                                        cockpit(this),
                                                                        buttons(this, Ui::LayoutAlignment::HORIZONTAL, Ui::ChildAlignment::MIDDLE, false),
                                                                        restart(&(this->buttons)),
                                                                        quit(&(this->buttons)) {
    this->msgQueue = &msgQueue;

    this->restart.setText("Restart");
    this->restart.setOnClick<EndGame>(this, &EndGame::restartClicked);
    this->quit.setText("Quit");
    this->quit.setOnClick<EndGame>(this, &EndGame::quitClicked);

    this->buttons.add(this->restart);
    this->buttons.add(this->quit);

    this->add(this->cockpit);
    this->add(this->buttons);
    this->hide();
}

EndGame::~EndGame() {
}

void EndGame::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::Resized) {
        this->cockpit.setSize(sf::Vector2f(parent->getWidth() / 1.2f, parent->getHeight() / 1.2f));
    }

    Layout::handleEvent(event);
}

void EndGame::setResult(GameResult gameResult, std::string team) {
    std::string pageNumber;
    if (gameResult == GameResult::DRAW) {
        pageNumber = this->DRAW_PAGE;
    } else if (gameResult == GameResult::WIN) {
        if (team == Game::REBELLION) {
            pageNumber = this->WIN_PAGE_REBELLION;
        } else {
            pageNumber = this->WIN_PAGE_EMPIRE;
        }
    } else {
        if (team == Game::REBELLION) {
            pageNumber = this->LOSE_PAGE_REBELLION;
        } else {
            pageNumber = this->LOSE_PAGE_EMPIRE;
        }
    }
    
    this->cockpit.setImage(Utils::joinPath({"res", team, "cockpit", pageNumber + ".png"}));
    this->cockpit.setSize(sf::Vector2f(parent->getWidth() / 1.2f, parent->getHeight() / 1.2f));
}


void EndGame::restartClicked() {
    this->msgQueue->send("endGameRestartClicked");
}

void EndGame::quitClicked() {
    this->msgQueue->send("endGameQuitClicked");
}