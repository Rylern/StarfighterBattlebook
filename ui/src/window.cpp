#include "window.hpp"



namespace Ui {
    Window::Window(sf::VideoMode mode, const std::string& title, uint32_t style, const sf::ContextSettings& settings):  sf::RenderWindow(mode, title, style, settings) {
        this->setVerticalSyncEnabled(true);

        sf::Vector2u size = this->getSize();
        Widget::setSize(sf::Vector2f((float) size.x, (float) size.y));

        this->background.setSize(sf::Vector2f((float) this->width, (float) this->height));
    } 	

    Window::~Window() {
        
    }


    void Window::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        if (this->isVisible) {
            states.transform *= getTransform();
            target.draw(this->background, states);
        }
    }

    void Window::handleEvent(const sf::Event& event) {
        Widget::handleEvent(event);
        
        if (this->isVisible) {
            if (event.type == sf::Event::Resized) {
                sf::Vector2f size = sf::Vector2f((float) event.size.width, (float) event.size.height);

                sf::FloatRect visibleArea(0, 0, size.x, size.y);
                this->setView(sf::View(visibleArea));

                Widget::setSize(sf::Vector2f(size.x, size.y));
                this->background.setSize(sf::Vector2f((float) this->width, (float) this->height));
            }
        } 
    }

    void Window::setImage(std::string imagePath) {
        this->texture.loadFromFile(imagePath);
        this->showImage();
    }

    void Window::showImage() {
        this->background.setTexture(&texture);
    }

    void Window::hideImage() {
        this->background.setTexture(nullptr);
    }
}