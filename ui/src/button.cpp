#include "button.hpp"
#include "utils.hpp"


namespace Ui {
    Button::Button(Widget* parent): Widget(parent) {
        
    }

    Button::~Button() {}


    void Button::setText(std::string label, int characterSize) {
        if (label != "") {
            this->font.loadFromFile(Utils::joinPath({"res", "DejaVuSans.ttf"}));

            this->text.setFont(font);
            this->text.setFillColor(sf::Color(253, 183, 72));
            this->text.setStyle(sf::Text::Bold);
            this->text.setCharacterSize(characterSize);
            this->text.setString(label);
            this->text.move(10, 3);

            sf::FloatRect backgroundRect = this->text.getLocalBounds();
            this->setSize(sf::Vector2f(backgroundRect.width + 25, backgroundRect.height + 21));
            this->background.setFillColor(sf::Color(81, 116, 182));
            this->background.setOutlineColor(sf::Color::White);
            this->background.setOutlineThickness(2);
        }
    }

    void Button::setImage(std::string imagePath) {
        this->texture.loadFromFile(imagePath);

        this->background.setTexture(&texture);
        sf::Vector2u textureSize = this->texture.getSize();
        this->background.setOutlineColor(sf::Color::Black);
        this->background.setOutlineThickness(2);

        this->setSize(sf::Vector2f((float) textureSize.x, (float) textureSize.y));
    }

    void Button::setSize(sf::Vector2f size) {
        this->background.setSize(size);
        Widget::setSize(size);
    }

    void Button::resetOutlineColor() {
        this->background.setOutlineColor(sf::Color::Black);
    }

    void Button::setOutlineColor(sf::Color color) {
        this->background.setOutlineColor(color);
    }


    void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (this->isVisible) {
            states.transform *= getTransform();
            target.draw(this->background, states);
            target.draw(this->text, states);
        }
    }

    void Button::handleEvent(const sf::Event& event) {
        Widget::handleEvent(event);

        if (this->isVisible) {
            sf::Vector2f cursorPositionProjected;
            switch (event.type) {
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        cursorPositionProjected = sf::Vector2f(event.mouseButton.x - getPosition().x, event.mouseButton.y - getPosition().y);
                        if (this->parent != nullptr) {
                            cursorPositionProjected.x -= this->parent->getPosition().x;
                            cursorPositionProjected.y -= this->parent->getPosition().y;
                        }
                        if (this->background.getGlobalBounds().contains(cursorPositionProjected)) {
                            if (this->onClick) {
                                this->onClick();
                            }
                        }
                    }
                    break;

                default:
                    break;
            }
        }
    }
}