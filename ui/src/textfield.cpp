#include "textfield.hpp"
#include "utils.hpp"

namespace Ui {
    TextField::TextField(Widget* parent): Widget(parent) {
        this->font.loadFromFile(Utils::joinPath({"res", "DejaVuSans.ttf"}));
        this->textLabel.setFont(font);
        this->textInput.setFont(font);

        this->textLabel.setFillColor(sf::Color(253, 183, 72));
        this->textLabel.setStyle(sf::Text::Bold);

        this->backgroundLabel.setFillColor(sf::Color(81, 116, 182));
        this->backgroundInput.setFillColor(sf::Color::Black);
    }

    TextField::~TextField() {

    }

    void TextField::setText(std::string label, int characterSize) {
        this->textLabel.setCharacterSize(characterSize);
        this->textLabel.setString(label);
        this->textLabel.move(10, 3);

        sf::FloatRect backgroundRect = this->textLabel.getLocalBounds();
        this->backgroundLabel.setSize(sf::Vector2f(backgroundRect.width + 25, backgroundRect.height + 21));

        this->backgroundInput.move(this->backgroundLabel.getSize().x, 0);
        this->textInput.move(this->backgroundLabel.getSize().x + 10, 4);

        this->setSize(sf::Vector2f(this->backgroundLabel.getSize().x + this->backgroundInput.getSize().x, this->backgroundLabel.getSize().y));
    }

    void TextField::setMaxInput(std::string maxInput) {
        this->maxNumberOfChars = (int) maxInput.size();

        this->textInput.setString(maxInput);
        sf::FloatRect backgroundRect = this->textInput.getLocalBounds();
        this->backgroundInput.setSize(sf::Vector2f(backgroundRect.width + 25, this->backgroundLabel.getSize().y));
        this->textInput.setString("");

        this->setSize(sf::Vector2f(this->backgroundLabel.getSize().x + this->backgroundInput.getSize().x, this->backgroundLabel.getSize().y));
    }


    void TextField::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (this->isVisible) {
            states.transform *= getTransform();
            target.draw(this->backgroundLabel, states);
            target.draw(this->backgroundInput, states);
            target.draw(this->textLabel, states);
            target.draw(this->textInput, states);
        }
    }

    void TextField::handleEvent(const sf::Event& event) {
        Widget::handleEvent(event);
        
        if (this->isVisible) {
            sf::String input = this->textInput.getString();
            int inputSize = (int) input.getSize();
            switch (event.type) {
                case sf::Event::TextEntered:
                    if (inputSize < this->maxNumberOfChars && ((event.text.unicode > 47 && event.text.unicode < 58) || event.text.unicode == 46)) {
                        this->textInput.setString(input + event.text.unicode);
                    }
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Backspace && inputSize > 0) {
                        this->textInput.setString(input.substring(0, inputSize-1));
                    }
                    break;
                default:
                    break;
            }
        }
    }

    void TextField::setInput(std::string input) {
        this->textInput.setString(input);
    }

    std::string TextField::getInput() const {
        return this->textInput.getString();
    }
}