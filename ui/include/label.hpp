#ifndef LABEL_H
#define LABEL_H

#include "widget.hpp"


namespace Ui {
    class Label: public Widget {

    public:
        Label(Widget* parent = nullptr);
        ~Label();
        void setText(std::string label, int characterSize = 36);
        void setBackgroundColor(sf::Color color);
        void setTextColor(sf::Color color);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void handleEvent(const sf::Event& event);

    private:
        sf::Font font;
        sf::Text text;
        sf::RectangleShape background;
    };
}



#endif