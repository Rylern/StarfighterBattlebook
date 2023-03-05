#ifndef GAME_H
#define GAME_H

#include "layout.hpp"
#include "button.hpp"
#include "label.hpp"
#include "image.hpp"
#include "soundmanager.hpp"
#include "msgqueue.hpp"

enum Team {
    UNKNOWN,
    REBELLION,
    EMPIRE
};


class Game: public Ui::Layout {

public:
    inline static const std::string REBELLION = "rebellion";
    inline static const std::string EMPIRE = "empire";

    Game(SoundManager& soundManager, MsgQueue<std::string>& msgQueue, Widget* parent = nullptr);
    ~Game();
    void handleEvent(const sf::Event& event);
    void setTeam(Team team);
    std::string getTeam() const;
    void newTurn(std::string turnInformation);
    void maneuverAccepted();
    void chooseHint(std::string hints);
    void hintAccepted();
    void hintNotAccepted();
    void seeHints(std::string hints);

private:
    SoundManager* soundManager;
    MsgQueue<std::string>* msgQueue;
    std::string team;
    bool inWarningLastTurn;

    Ui::Layout dashboard;
    Ui::Layout indicators;
    Ui::Label warning;
    Ui::Label lockOn;
    Ui::Image cockpit;
    Ui::Layout information;
    Ui::Label HP;
    Ui::Label damage;
    Ui::Layout maneuversContainer;
    Ui::Label status;

    void maneuverClicked(int maneuver);
    void setCockpit(std::string pageNumber);
    std::string getOtherTeam();
};



#endif