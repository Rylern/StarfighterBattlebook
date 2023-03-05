#ifndef WINDOW_H
#define WINDOW_H

#include "widget.hpp"
#include <SFML/Graphics.hpp>
#include <cstdint>


namespace Ui {
    class Window: public sf::RenderWindow, public Widget {

    public:
        using sf::RenderWindow::draw;
        using Widget::draw;

        Window(sf::VideoMode mode, const std::string& title, uint32_t style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());
        ~Window();

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void handleEvent(const sf::Event& event);

        void setImage(std::string imagePath);
        void showImage();
        void hideImage();


    private:
        sf::Texture texture;
        sf::RectangleShape background;
    };
}

#endif