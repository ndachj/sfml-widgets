#include "FormLayout.hpp"
#include "Gui/Theme.hpp"
#include "Gui/Label.hpp"

namespace gui
{

FormLayout::FormLayout():
    m_labelWidth(0.f)
{
}


Widget* FormLayout::addRow(const sf::String& str, Widget* widget)
{
    gui::Label* label = new gui::Label(str);
    if (label->getSize().x > m_labelWidth)
    {
        m_labelWidth = label->getSize().x;
    }
    Layout::add(label);
    Layout::add(widget);
    return widget;
}


void FormLayout::recomputeGeometry()
{
    size_t i = 0;
    sf::Vector2f pos;
    sf::Vector2f size;
    for (Widget* widget = getFirstWidget(); widget != nullptr; widget = widget->m_next)
    {
        if (i % 2 == 0)
        {
            // Left-side: label
            widget->setPosition(0, pos.y);
            if (widget->getSize().x > m_labelWidth)
                m_labelWidth = widget->getSize().x;
        }
        else
        {
            // Right-side: widget
            widget->setPosition(m_labelWidth + Theme::MARGIN, pos.y);
            // Row height is at least Theme::getBoxHeight()
            pos.y += std::max(widget->getSize().y, Theme::getBoxHeight()) + Theme::MARGIN;
            if (widget->getSize().x > size.x)
                size.x = widget->getSize().x;
        }
        ++i;
    }
    size.x += m_labelWidth + Theme::MARGIN;
    size.y = pos.y - Theme::MARGIN;
    Widget::setSize(size);
}

}
