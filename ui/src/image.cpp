#include "image.hpp"



namespace Ui {
    Image::Image(Widget* parent): Widget(parent) {
        
    }

    Image::~Image() {}

    void Image::setImage(std::string imagePath) {
        this->texture.loadFromFile(imagePath);

        this->image.setTexture(&texture);
        sf::Vector2u textureSize = this->texture.getSize();

        this->setSize(sf::Vector2f((float) textureSize.x, (float) textureSize.y));
    }


    void Image::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (this->isVisible) {
            states.transform *= getTransform();
            target.draw(this->image, states);
        }
    }

    void Image::handleEvent(const sf::Event& event) {
        Widget::handleEvent(event);
    }

    void Image::setSize(sf::Vector2f size) {
        this->image.setSize(size);
        Widget::setSize(size);
    }
}