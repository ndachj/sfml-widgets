#include "Layout.hpp"
#include "FormLayout.hpp"
#include "HBoxLayout.hpp"
#include "VBoxLayout.hpp"
#include "Gui/Button.hpp"
#include "Gui/Label.hpp"

namespace gui
{

Layout::Layout():
    m_first(nullptr),
    m_last(nullptr),
    m_hover(nullptr),
    m_focus(nullptr)
{
}


Layout::~Layout()
{
    // Deallocate all widgets
    const Widget* widget = m_first;
    while (widget != nullptr)
    {
        const Widget* next = widget->m_next;
        delete widget;
        widget = next;
    }
}


Widget* Layout::add(Widget* widget)
{
    widget->setParent(this);

    if (m_first == nullptr)
    {
        m_first = m_last = widget;
    }
    else
    {
        m_last->m_next = widget;
        widget->m_previous = m_last;
        m_last = widget;
    }
    recomputeGeometry();
    return widget;
}


Button* Layout::addButton(const sf::String& string, std::function<void(void)> callback)
{
    Button* button = new Button(string);
    button->setCallback(callback);
    add(button);
    return button;
}


Label* Layout::addLabel(const sf::String& string)
{
    Label* label = new Label(string);
    add(label);
    return label;
}


FormLayout* Layout::addFormLayout()
{
    FormLayout* form = new FormLayout();
    add(form);
    return form;
}


HBoxLayout* Layout::addHBoxLayout()
{
    HBoxLayout* hbox = new HBoxLayout();
    add(hbox);
    return hbox;
}


VBoxLayout* Layout::addVBoxLayout()
{
    VBoxLayout* vbox = new VBoxLayout();
    add(vbox);
    return vbox;
}


Widget* Layout::getFirstWidget()
{
    return m_first;
}


// Callbacks -------------------------------------------------------------------

void Layout::onStateChanged(State state)
{
    if (state == StateDefault)
    {
        if (m_focus != nullptr)
        {
            m_focus->setState(StateDefault);
            m_focus = nullptr;
        }
    }
}


void Layout::onMouseMoved(float x, float y)
{
    // Pressed widgets still receive mouse move events even when not hovered if mouse is pressed
    // Example: moving a slider's handle
    if (m_focus != nullptr && sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        m_focus->onMouseMoved(x - m_focus->getPosition().x, y - m_focus->getPosition().y);
        if (!m_focus->containsPoint({x, y}))
        {
            m_hover = nullptr;
        }
    }
    else
    {
        // Loop over widgets
        for (Widget* widget = m_first; widget != nullptr; widget = widget->m_next)
        {
            // Convert mouse position to widget coordinates system
            sf::Vector2f mouse = sf::Vector2f(x, y) - widget->getPosition();
            if (widget->containsPoint(mouse))
            {
                if (m_hover != widget)
                {
                    // A new widget is hovered
                    if (m_hover != nullptr)
                    {
                        m_hover->setState(StateDefault);
                        m_hover->onMouseLeave();
                    }

                    m_hover = widget;
                    // Don't send Hovered state if widget is already focused
                    if (m_hover != m_focus)
                    {
                        widget->setState(StateHovered);
                    }
                    widget->onMouseEnter();
                }
                else
                {
                    // Hovered widget was already hovered
                    widget->onMouseMoved(mouse.x, mouse.y);
                }
                return;
            }
        }
        // No widget hovered, remove hovered state
        if (m_hover != nullptr)
        {
            m_hover->onMouseMoved(x, y);
            m_hover->setState(m_focus == m_hover ? StateFocused : StateDefault);
            m_hover->onMouseLeave();
            m_hover = nullptr;
        }
    }
}


void Layout::onMousePressed(float x, float y)
{
    if (m_hover != nullptr)
    {
        // Clicked widget takes focus
        if (m_focus != m_hover)
        {
            focusWidget(m_hover);
        }
        // Send event to widget
        sf::Vector2f mouse = sf::Vector2f(x, y) - m_hover->getPosition();
        m_hover->onMousePressed(mouse.x, mouse.y);
    }
    // User didn't click on a widget, remove focus state
    else if (m_focus != nullptr)
    {
        m_focus->setState(StateDefault);
        m_focus = nullptr;
    }
}


void Layout::onMouseReleased(float x, float y)
{
    if (m_focus != nullptr)
    {
        // Send event to the focused widget
        sf::Vector2f mouse = sf::Vector2f(x, y) - m_focus->getPosition();
        m_focus->onMouseReleased(mouse.x, mouse.y);
        m_focus->setState(StateFocused);
    }
}


void Layout::onMouseWheelMoved(int delta)
{
    if (m_focus != nullptr)
    {
        m_focus->onMouseWheelMoved(delta);
    }
}


void Layout::onKeyPressed(const sf::Event::KeyEvent& key)
{
    if (key.code == Theme::nextWidgetKey)
    {
        if (!focusNextWidget())
            // Try to focus first widget if possible
            focusNextWidget();
    }
    else if (key.code == Theme::previousWidgetKey)
    {
        if (!focusPreviousWidget())
            focusPreviousWidget();
    }
    else if (m_focus != nullptr)
    {
        m_focus->onKeyPressed(key);
    }
}


void Layout::onKeyReleased(const sf::Event::KeyEvent& key)
{
    if (m_focus != nullptr)
    {
        m_focus->onKeyReleased(key);
    }
}


void Layout::onTextEntered(sf::Uint32 unicode)
{
    if (m_focus != nullptr)
    {
        m_focus->onTextEntered(unicode);
    }
}


void Layout::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (const Widget* widget = m_first; widget != nullptr; widget = widget->m_next)
    {
        target.draw(*widget, states);
    }
}


bool Layout::focusWidget(Widget* widget)
{
    if (widget != nullptr)
    {
        // If another widget was already focused, remove focus
        if (m_focus != nullptr && m_focus != widget)
        {
            m_focus->setState(StateDefault);
            m_focus = nullptr;
        }
        // Apply focus to widget
        if (widget->isSelectable())
        {
            m_focus = widget;
            widget->setState(StateFocused);
            return true;
        }
    }
    return false;
}


bool Layout::focusPreviousWidget()
{
    // If a sublayout is already focused
    if (m_focus != nullptr && m_focus->toLayout() != nullptr)
    {
        if (m_focus->toLayout()->focusPreviousWidget())
            return true;
    }

    Widget* start = m_focus != nullptr ? m_focus->m_previous : m_last;
    for (Widget* widget = start; widget != nullptr; widget = widget->m_previous)
    {
        Layout* container = widget->toLayout();
        if (container != nullptr)
        {
            if (container->focusPreviousWidget())
            {
                focusWidget(container);
                return true;
            }
        }
        else if (focusWidget(widget))
        {
            return true;
        }
    }

    if (m_focus != nullptr)
        m_focus->setState(StateDefault);
    m_focus = nullptr;
    return false;
}


bool Layout::focusNextWidget()
{
    // If a sublayout is already focused
    if (m_focus != nullptr && m_focus->toLayout() != nullptr)
    {
        if (m_focus->toLayout()->focusNextWidget())
            return true;
    }

    Widget* start = m_focus != nullptr ? m_focus->m_next : m_first;
    for (Widget* widget = start; widget != nullptr; widget = widget->m_next)
    {
        Layout* container = widget->toLayout();
        if (container != nullptr)
        {
            if (container->focusNextWidget())
            {
                focusWidget(container);
                return true;
            }
        }
        else if (focusWidget(widget))
        {
            return true;
        }
    }

    if (m_focus != nullptr)
        m_focus->setState(StateDefault);
    m_focus = nullptr;
    return false;
}

}
