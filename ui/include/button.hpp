#ifndef BUTTON_H
#define BUTTON_H

#include "widget.hpp"
#include <functional>


namespace Ui {
    class Button: public Widget {

    public:
        Button(Widget* parent = nullptr);
        ~Button();
        void setText(std::string label, int characterSize = 36);
        void setImage(std::string imagePath);
        void setSize(sf::Vector2f size);
        void resetOutlineColor();
        void setOutlineColor(sf::Color color);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void handleEvent(const sf::Event& event);
        template <class T> void setOnClick(T* handler, std::function<void(T*)> function) {
            this->onClick = std::bind(function, handler);
        }

    private:
        sf::Font font;
        sf::Text text;
        sf::Texture texture;
        sf::RectangleShape background;
        std::function<void()> onClick;
    };
}

#endif