#include "inputhandler.h"

InputHandler::InputHandler(QObject* parent)
    : QObject(parent)
{
}

bool InputHandler::isKeyPressed(int key) const
{
    return pressedKeys.contains(key);
}

bool InputHandler::isButtonPressed(Qt::MouseButton button) const
{
    return pressedButtons.contains(button);
}

QPoint InputHandler::getMousePosition() const
{
    return mousePosition;
}

bool InputHandler::eventFilter(QObject* obj, QEvent* event)
{
    switch (event->type())
    {
    case QEvent::KeyPress:
    {
        QKeyEvent* e = static_cast<QKeyEvent*>(event);
        pressedKeys.insert(e->key());
        emit keyPressed(e->key());
        return true;
    }
    case QEvent::KeyRelease:
    {
        QKeyEvent* e = static_cast<QKeyEvent*>(event);
        pressedKeys.remove(e->key());
        emit keyReleased(e->key());
        return true;
    }
    case QEvent::MouseMove:
    {
        QMouseEvent* e = static_cast<QMouseEvent*>(event);
        mousePosition = e->position().toPoint(); // Change this line
        emit mouseMoved(mousePosition);
        return true;
    }
    case QEvent::MouseButtonPress:
    {
        QMouseEvent* e = static_cast<QMouseEvent*>(event);
        pressedButtons.insert(e->button());
        emit mouseButtonPressed(e->button(), e->position().toPoint()); // Change this line
        return true;
    }
    case QEvent::MouseButtonRelease:
    {
        QMouseEvent* e = static_cast<QMouseEvent*>(event);
        pressedButtons.remove(e->button());
        emit mouseButtonReleased(e->button(), e->position().toPoint());
        return true;
    }
    case QEvent::Wheel:
    {
        QWheelEvent* e = static_cast<QWheelEvent*>(event);
        emit mouseWheelMoved(e->angleDelta().y());
        return true;
    }
    default:
        return QObject::eventFilter(obj, event);
    }
}
