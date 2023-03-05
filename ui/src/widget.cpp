#include "widget.hpp"
#include <iostream>


namespace Ui {
    Widget::Widget(Widget* parentWidget):   parent(parentWidget),
                                            height(0),
                                            width(0),
                                            isVisible(true) {
                                                
    }

    Widget::~Widget() {

    }

    void Widget::setPositionRelativeY(float positionY) {
        this->setPosition(this->getPosition().x, this->parent->getPosition().y + this->parent->getHeight() * positionY);
    }

    unsigned int Widget::getHeight() const {
        return this->height;
    }

    unsigned int Widget::getWidth() const {
        return this->width;
    }

    void Widget::hide() {
        this->isVisible = false;
    }

    void Widget::show() {
        this->isVisible = true;
    }

    void Widget::setName(std::string newName) {
        this->name = newName;
    }

    std::string Widget::getName() const {
        return this->name;
    }
    
    void Widget::handleEvent(const sf::Event& event) {
        (void) event;
    }

    void Widget::setSize(sf::Vector2f size) {
        if (this->name == "cockpit") {
            std::cout << size.y << std::endl;
        }

        this->width = (unsigned int) size.x;
        this->height = (unsigned int) size.y;
        if (this->parent != nullptr) {
            this->parent->sizeChanged();
        }
    }

    void Widget::sizeChanged() {

    }
}