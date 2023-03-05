#include "layout.hpp"
#include <algorithm>


namespace Ui {
    Layout::Layout(Widget* parent, LayoutAlignment layoutAlignment, ChildAlignment childAlignment, bool isExpanding): Widget(parent) {
        this->layoutAlignment = layoutAlignment;
        this->childAlignment = childAlignment;
        this->isExpanding = isExpanding;
        this->childMargin = 0;
        this->preferredHeight = 0;
        this->preferredWidth = 0;
    }

    Layout::~Layout() {

    }

    void Layout::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (this->isVisible) {
            states.transform *= getTransform();
            for (auto i = this->children.begin(); i != this->children.end(); ++i) {
                target.draw(**i, states);
            }
        }
    }

    void Layout::handleEvent(const sf::Event& event) {
        Widget::handleEvent(event);

        if (this->isVisible) {
            for (auto i = this->children.begin(); i != this->children.end(); ++i) {
                (*i)->handleEvent(event);
            }    
        }
        if (event.type == sf::Event::Resized) {
            this->setChildrenPosition();
        }
    }

    void Layout::add(Widget& child) {
        this->children.push_back(&child);

        this->setChildrenPosition();
    }

    std::vector<Widget*> Layout::getChildren() {
        return this->children;
    }

    void Layout::setChildMargin(float newChildMargin) {
        this->childMargin = newChildMargin;
    }

    void Layout::setSize(sf::Vector2f size) {
        this->preferredWidth = (unsigned int) size.x;
        this->preferredHeight = (unsigned int) size.y;
        Widget::setSize(size);
    }

    void Layout::clear() {
        for (auto i = this->children.begin(); i != this->children.end(); ++i) {
            delete (*i);
        }
        this->children.clear();
    }

    void Layout::setChildrenPosition() {
        this->width = this->preferredWidth;
        this->height = this->preferredHeight;
        if (this->isExpanding && this->parent != nullptr) {
            this->setSize(sf::Vector2f((float) this->parent->getWidth(), (float) this->parent->getHeight()));
        }

        if (this->layoutAlignment == LayoutAlignment::VERTICAL) {
            unsigned int childrenHeight = 0;

            for (auto i = this->children.begin(); i != this->children.end(); ++i) {
                childrenHeight += (*i)->getHeight();
                this->width = std::max((*i)->getWidth(), this->width);
            }
            childrenHeight += (unsigned int) (this->childMargin * (this->children.size() - 1));

            this->height = std::max(childrenHeight, this->height);

            float currentHeight = (this->height - childrenHeight) / 2.f;

            for (auto i = this->children.begin(); i != this->children.end(); ++i) {
                float newPositionX = 0;
                switch (this->childAlignment) {
                    case ChildAlignment::BEGIN:
                        break;
                    case ChildAlignment::MIDDLE:
                        newPositionX += (this->width - (*i)->getWidth()) / 2;
                        break;
                    case ChildAlignment::END:
                        newPositionX += this->width - (*i)->getWidth();
                        break;
                    default:
                        break;
                }

                (*i)->setPosition(newPositionX, currentHeight);
                currentHeight += (*i)->getHeight() + this->childMargin;
            }
        } else {
            unsigned int childrenWidth = 0;

            for (auto i = this->children.begin(); i != this->children.end(); ++i) {
                childrenWidth += (*i)->getWidth();
                this->height = std::max((*i)->getHeight(), this->height);
            }
            childrenWidth += (unsigned int) (this->childMargin * (this->children.size() - 1));

            this->width = std::max(childrenWidth, this->width);

            float currentWidth = (this->width - childrenWidth) / 2.f;

            for (auto i = this->children.begin(); i != this->children.end(); ++i) {
                float newPositionY = 0;
                switch (this->childAlignment) {
                    case ChildAlignment::BEGIN:
                        break;
                    case ChildAlignment::MIDDLE:
                        newPositionY += (this->height - (*i)->getHeight()) / 2;
                        break;
                    case ChildAlignment::END:
                        newPositionY += this->height - (*i)->getHeight();
                        break;
                    default:
                        break;
                }

                (*i)->setPosition(currentWidth, newPositionY);
                currentWidth += (*i)->getWidth() + this->childMargin;
            }
        }
    }

    void Layout::sizeChanged() {
        this->setChildrenPosition();
    }
}