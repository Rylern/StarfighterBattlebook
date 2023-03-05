#include "label.hpp"
#include "utils.hpp"


namespace Ui {
    Label::Label(Widget* parent): Widget(parent) {
        this->font.loadFromFile(Utils::joinPath({"res", "DejaVuSans.ttf"}));

        this->text.setFont(font);
        this->text.setFillColor(sf::Color::White);
        this->text.setStyle(sf::Text::Bold);
        this->text.move(10, 3);

        this->background.setFillColor(sf::Color(81, 116, 182));
    }

    Label::~Label() {}

    void Label::setText(std::string label, int characterSize) {
        this->text.setString(label);
            this->text.setCharacterSize(characterSize);
        
        if (label == "") {
            this->background.setSize(sf::Vector2f(0, 0));
        } else {
            sf::FloatRect backgroundRect = this->text.getLocalBounds();
            this->background.setSize(sf::Vector2f(backgroundRect.width + 25, backgroundRect.height + 21));
        }

        this->setSize(this->background.getSize());
    }

    void Label::setBackgroundColor(sf::Color color) {
        this->background.setFillColor(color);
    }

    void Label::setTextColor(sf::Color color) {
        this->text.setFillColor(color);
    }

    void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (this->isVisible) {
            states.transform *= getTransform();
            target.draw(this->background, states);
            target.draw(this->text, states);
        }
    }

    void Label::handleEvent(const sf::Event& event) {
        Widget::handleEvent(event);

        if (this->isVisible) {
            
        }
    }
}