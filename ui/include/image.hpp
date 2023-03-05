#ifndef IMAGE_H
#define IMAGE_H

#include "widget.hpp"


namespace Ui {
    class Image: public Widget {

    public:
        Image(Widget* parent = nullptr);
        ~Image();
        void setImage(std::string imagePath);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void handleEvent(const sf::Event& event);
        void setSize(sf::Vector2f size);

    private:
        sf::Texture texture;
        sf::RectangleShape image;
    };
}



#endif