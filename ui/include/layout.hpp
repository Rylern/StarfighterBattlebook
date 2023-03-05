#ifndef LAYOUT_H
#define LAYOUT_H


#include "widget.hpp"
#include <vector>

namespace Ui {

    enum LayoutAlignment {
        HORIZONTAL,
        VERTICAL
    };

    enum ChildAlignment {
        BEGIN,
        MIDDLE,
        END
    };

    class Layout: public Widget {

    public:
        Layout(Widget* parent = nullptr, LayoutAlignment layoutAlignment = LayoutAlignment::VERTICAL, ChildAlignment childAlignment = ChildAlignment::MIDDLE, bool isExpanding = true);
        ~Layout();

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void handleEvent(const sf::Event& event);

        void add(Widget& child);
        std::vector<Widget*> getChildren();
        void setChildMargin(float childMargin);
        void setSize(sf::Vector2f size);
        void clear();

    protected:
        void setChildrenPosition();

    private:
        void sizeChanged();

        LayoutAlignment layoutAlignment;
        ChildAlignment childAlignment;
        std::vector<Widget*> children;
        float childMargin;
        bool isExpanding;
        unsigned int preferredHeight;
        unsigned int preferredWidth;
    };

}

#endif