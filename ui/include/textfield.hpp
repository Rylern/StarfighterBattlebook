#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include "widget.hpp"


namespace Ui {
    class TextField: public Widget {

    public:
        TextField(Widget* parent = nullptr);
        ~TextField();
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void handleEvent(const sf::Event& event);
        void setText(std::string label, int characterSize = 36);
        void setMaxInput(std::string maxInput);
        void setInput(std::string input);
        std::string getInput() const;

    private:
        int maxNumberOfChars;
        sf::Font font;
        sf::Text textLabel;
        sf::RectangleShape backgroundLabel;
        sf::Text textInput;
        sf::RectangleShape backgroundInput;
    };
}



#endif