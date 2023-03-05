#include "game.hpp"
#include "utils.hpp"


Game::Game(SoundManager& soundManager, MsgQueue<std::string>& msgQueue, Ui::Widget* parent):
        Ui::Layout(parent),
        inWarningLastTurn(false),
        dashboard(this, Ui::LayoutAlignment::HORIZONTAL, Ui::ChildAlignment::MIDDLE, false),
        indicators(&(this->dashboard), Ui::LayoutAlignment::VERTICAL, Ui::ChildAlignment::MIDDLE, false),
        warning(&(this->indicators)),
        lockOn(&(this->indicators)),
        cockpit(&(this->dashboard)),
        information(&(this->dashboard), Ui::LayoutAlignment::VERTICAL, Ui::ChildAlignment::MIDDLE, false),
        HP(&(this->information)),
        damage(&(this->information)),
        maneuversContainer(this, Ui::LayoutAlignment::HORIZONTAL, Ui::ChildAlignment::MIDDLE, false),
        status(this) {
    this->soundManager = &soundManager;
    this->msgQueue = &msgQueue;

    this->warning.setText("WARNING", 30);
    this->warning.setBackgroundColor(sf::Color(124, 128, 125));
    this->warning.setTextColor(sf::Color(60, 60, 60));
    this->lockOn.setText("LOCK-ON", 30);
    this->lockOn.setBackgroundColor(sf::Color(124, 128, 125));
    this->lockOn.setTextColor(sf::Color(60, 60, 60));
    this->indicators.setChildMargin(15);
    this->indicators.add(this->warning);
    this->indicators.add(this->lockOn);

    this->HP.setText("", 16);
    this->HP.setBackgroundColor(sf::Color::Transparent);
    this->HP.setTextColor(sf::Color::Black);
    this->damage.setText("", 16);
    this->damage.setBackgroundColor(sf::Color::Transparent);
    this->information.setSize(sf::Vector2f((float) this->indicators.getWidth(), (float) this->information.getHeight()));
    this->information.setChildMargin(15);
    this->information.add(this->HP);
    this->information.add(this->damage);

    this->dashboard.add(this->indicators);
    this->dashboard.add(this->cockpit);
    this->dashboard.add(this->information);

    this->maneuversContainer.setChildMargin(2);
    
    this->status.setText("", 20);
    this->status.setBackgroundColor(sf::Color::Transparent);
    this->status.setTextColor(sf::Color::Black);

    this->add(this->dashboard);
    this->add(this->maneuversContainer);
    this->add(this->status);
    this->hide();
}

Game::~Game() {
    this->maneuversContainer.clear();
}

void Game::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::Resized) {
        this->cockpit.setSize(sf::Vector2f(parent->getWidth()/1.8f, parent->getHeight()/1.2f));

        std::vector<Widget*> maneuvers = this->maneuversContainer.getChildren();
        sf::Vector2f maneuverSize = sf::Vector2f((float) (parent->getWidth() / (maneuvers.size() + 1)), parent->getHeight() / 12.f);
        for (auto maneuver = maneuvers.begin(); maneuver != maneuvers.end(); ++maneuver) {
            (*maneuver)->setSize(maneuverSize);
        }
    }

    Layout::handleEvent(event);
}


void Game::maneuverClicked(int maneuver) {
    this->msgQueue->send("maneuver:" + std::to_string(maneuver));
}

void Game::setCockpit(std::string pageNumber) {
    this->cockpit.setImage(Utils::joinPath({"res", this->team, "cockpit", pageNumber + ".png"}));
    this->cockpit.setSize(sf::Vector2f(parent->getWidth() / 1.8f, parent->getHeight() / 1.2f));
}

void Game::setTeam(Team newTeam) {
    this->maneuversContainer.clear();

    if (newTeam != Team::UNKNOWN) {
        int nbManeuvers;

        if (newTeam == Team::REBELLION) {
            this->team = this->REBELLION;
            nbManeuvers = 22;
        } else {
            this->team = this->EMPIRE;
            nbManeuvers = 21;
        }

        sf::Vector2f maneuverSize = sf::Vector2f((float) (parent->getWidth() / (nbManeuvers + 1)), parent->getHeight() / 12.f);
        for (int i=0; i<nbManeuvers; i++) {
            Ui::Button* maneuver = new Ui::Button(&(this->maneuversContainer));
            maneuver->setImage(Utils::joinPath({"res", this->team, "maneuvers", std::to_string(i) + ".png"}));
            maneuver->setOnClick<Game>(this, std::bind(&Game::maneuverClicked, this, i));
            maneuver->setSize(maneuverSize);
            this->maneuversContainer.add(*maneuver);
        }

        this->setChildrenPosition();
    }
}

std::string Game::getOtherTeam() {
    return this->team == this->REBELLION ? this->EMPIRE : this->REBELLION;
}

std::string Game::getTeam() const {
    return this->team;
}

void Game::newTurn(std::string turnInformation) {
    this->soundManager->flying(this->team);

    std::string delimiter = "-";
    size_t pos = 0;
    std::string token;

    pos = turnInformation.find(delimiter);
    std::string pageNumber = turnInformation.substr(0, pos);
    turnInformation.erase(0, pos + delimiter.length());

    pos = turnInformation.find(delimiter);
    int damageDealt = std::stoi(turnInformation.substr(0, pos));
    turnInformation.erase(0, pos + delimiter.length());

    pos = turnInformation.find(delimiter);
    int damageTaken = std::stoi(turnInformation.substr(0, pos));
    turnInformation.erase(0, pos + delimiter.length());

    pos = turnInformation.find(delimiter);
    int warningNumber = std::stoi(turnInformation.substr(0, pos));
    turnInformation.erase(0, pos + delimiter.length());

    pos = turnInformation.find(delimiter);
    int lockOnNumber = std::stoi(turnInformation.substr(0, pos));
    turnInformation.erase(0, pos + delimiter.length());

    pos = turnInformation.find(delimiter);
    int view = std::stoi(turnInformation.substr(0, pos));
    turnInformation.erase(0, pos + delimiter.length());

    pos = turnInformation.find(delimiter);
    int range = std::stoi(turnInformation.substr(0, pos));
    turnInformation.erase(0, pos + delimiter.length());

    pos = turnInformation.find(delimiter);
    int HPNumber = std::stoi(turnInformation.substr(0, pos));
    turnInformation.erase(0, pos + delimiter.length());

    pos = turnInformation.find(delimiter);
    int playerInWarning = std::stoi(turnInformation.substr(0, pos));

    this->setCockpit(pageNumber);

    this->damage.setText("", 16);
    if (damageDealt != 0) {
        this->damage.setText("+" + std::to_string(damageDealt), 16);
        this->damage.setTextColor(sf::Color(0, 150, 0));
        this->soundManager->lasers(this->team);
    }
    if (damageTaken != 0) {
        this->damage.setText("-" + std::to_string(damageTaken), 16);
        this->damage.setTextColor(sf::Color(200, 0, 0));
        this->soundManager->lasers(this->getOtherTeam());
    }

    this->warning.setBackgroundColor(sf::Color(124, 128, 125));
    if (warningNumber != 0) {
        this->warning.setBackgroundColor(sf::Color(200, 0, 0));
        this->warning.setTextColor(sf::Color::White);
    }
    this->lockOn.setBackgroundColor(sf::Color(124, 128, 125));
    if (lockOnNumber != 0) {
        this->lockOn.setBackgroundColor(sf::Color(0, 150, 0));
        this->lockOn.setTextColor(sf::Color::White);
    }

    (void)view;
    (void)range;

    this->HP.setText("HP: " + std::to_string(HPNumber), 16);

    std::string statusMessage = "Select a maneuver";
    if (playerInWarning == 1) {
        statusMessage += ".\nYou are in warning mode. Give a hint after you selected a maneuver";
        if (!inWarningLastTurn) {
            this->soundManager->onYourTail(this->team);
        }
    } else if (playerInWarning == 2) {
        statusMessage += ".\nYou are in lock-on mode. Wait for the hint before selecting a maneuver";
    }
    this->status.setText(statusMessage, 20);
    this->inWarningLastTurn = playerInWarning == 1;

    std::vector<Widget*> maneuvers = this->maneuversContainer.getChildren();
    for (auto maneuver = maneuvers.begin(); maneuver != maneuvers.end(); ++maneuver) {
        ((Ui::Button*) *maneuver)->resetOutlineColor();
    }
}

void Game::maneuverAccepted() {
    this->status.setText("Maneuver accepted", 20);
}

void Game::chooseHint(std::string hints) {
    std::vector<Widget*> maneuvers = this->maneuversContainer.getChildren();
    size_t pos = 0;
    std::string delimiter = "-";

    while ((pos = hints.find(delimiter)) != std::string::npos) {
        int hint = std::stoi(hints.substr(0, pos));
        ((Ui::Button*) maneuvers[hint])->setOutlineColor(sf::Color::Red);
        hints.erase(0, pos + delimiter.length());
    }
    int hint = std::stoi(hints);
    ((Ui::Button*) maneuvers[hint])->setOutlineColor(sf::Color::Red);

    this->status.setText("Choose a hint among those selected", 20);
}

void Game::hintAccepted() {
    std::vector<Widget*> maneuvers = this->maneuversContainer.getChildren();
    for (auto maneuver = maneuvers.begin(); maneuver != maneuvers.end(); ++maneuver) {
        ((Ui::Button*) *maneuver)->resetOutlineColor();
    }

    this->status.setText("Hint accepted", 20);
}

void Game::hintNotAccepted() {
    this->status.setText("Choose a hint among those selected. Invalid hint", 20);
}

void Game::seeHints(std::string hints) {
    std::vector<Widget*> maneuvers = this->maneuversContainer.getChildren();
    size_t pos = 0;
    std::string delimiter = "-";

    while ((pos = hints.find(delimiter)) != std::string::npos) {
        int hint = std::stoi(hints.substr(0, pos));
        ((Ui::Button*) maneuvers[hint])->setOutlineColor(sf::Color::Green);
        hints.erase(0, pos + delimiter.length());
    }
    int hint = std::stoi(hints);
    ((Ui::Button*) maneuvers[hint])->setOutlineColor(sf::Color::Green);

    this->status.setText("Select a maneuver.\nThe other player chose a maneuver among those selected", 20);
}