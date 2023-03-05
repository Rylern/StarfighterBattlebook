#include "teamselection.hpp"
#include "window.hpp"
#include "utils.hpp"



TeamSelection::TeamSelection(MsgQueue<std::string>& msgQueue, Ui::Widget* parent):  Ui::Layout(parent, Ui::LayoutAlignment::HORIZONTAL),
                                                                                    rebellion(this),
                                                                                    empire(this) {
    this->msgQueue = &msgQueue;

    sf::Vector2f buttonSize = sf::Vector2f(parent->getWidth()/2.f, (float) parent->getHeight());
    
    this->rebellion.setImage(Utils::joinPath({"res", "rebellion", "fighter.png"}));
    this->rebellion.setSize(buttonSize);
    this->rebellion.setOnClick<TeamSelection>(this, &TeamSelection::rebellionClicked);

    this->empire.setImage(Utils::joinPath({"res", "empire", "fighter.png"}));
    this->empire.setSize(buttonSize);
    this->empire.setOnClick<TeamSelection>(this, &TeamSelection::empireClicked);

    this->add(this->rebellion);
    this->add(this->empire);
    this->hide();
}

TeamSelection::~TeamSelection() {

}

void TeamSelection::rebellionClicked() {
    this->msgQueue->send("teamSelectionRebellionClicked");
}

void TeamSelection::empireClicked() {
    this->msgQueue->send("teamSelectionEmpireClicked");
}

void TeamSelection::handleEvent(const sf::Event& event) {
    Layout::handleEvent(event);

    if (event.type == sf::Event::Resized) {
        sf::Vector2f buttonSize = sf::Vector2f(parent->getWidth()/2.f, (float) parent->getHeight());
        this->rebellion.setSize(buttonSize);
        this->empire.setSize(buttonSize);
    }
}