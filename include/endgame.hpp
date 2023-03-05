#ifndef ENDGAME_H
#define ENDGAME_H

#include "layout.hpp"
#include "button.hpp"
#include "image.hpp"
#include "msgqueue.hpp"
#include "game.hpp"


enum GameResult {
    DRAW,
    WIN,
    LOSE
};


class EndGame: public Ui::Layout {

public:
    EndGame(MsgQueue<std::string>& msgQueue, Widget* parent = nullptr);
    ~EndGame();
    void handleEvent(const sf::Event& event);
    void setResult(GameResult gameResult, std::string team);

private:
    inline static const std::string DRAW_PAGE = "223";
    inline static const std::string WIN_PAGE_REBELLION = "16";
    inline static const std::string WIN_PAGE_EMPIRE = "2";
    inline static const std::string LOSE_PAGE_REBELLION = "2";
    inline static const std::string LOSE_PAGE_EMPIRE = "16";
    MsgQueue<std::string>* msgQueue;

    Ui::Image cockpit;
    Ui::Layout buttons;
    Ui::Button restart;
    Ui::Button quit;

    void restartClicked();
    void quitClicked();
};



#endif