#ifndef WIDGET_H
#define WIDGET_H

#include <SFML/Graphics.hpp>
#include <iostream>

namespace Ui {
    class Widget: public sf::Transformable, public sf::Drawable {

    public:
        Widget(Widget* parent = nullptr);
        ~Widget();
        void setPositionRelativeY(float positionY);
        unsigned int getHeight() const;
        unsigned int getWidth() const;
        void hide();
        void show();
        void setName(std::string name);
        std::string getName() const;

        virtual void handleEvent(const sf::Event& event);
        virtual void setSize(sf::Vector2f size);
        virtual void sizeChanged();


    protected:
        Widget* parent;
        unsigned int height;
        unsigned int width;
        bool isVisible;
        std::string name;


    private:
    };
}

#endif