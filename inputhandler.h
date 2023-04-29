#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <QObject>
#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>

class InputHandler : public QObject
{
    Q_OBJECT

public:
    explicit InputHandler(QObject* parent = nullptr);

    // Methods to check the current input status
    bool isKeyPressed(int key) const;
    bool isButtonPressed(Qt::MouseButton button) const;
    QPoint getMousePosition() const;

signals:
    // Signals for when inputs change
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(const QPoint& position);
    void mouseButtonPressed(Qt::MouseButton button, const QPoint& position);
    void mouseButtonReleased(Qt::MouseButton button, const QPoint& position);
    void mouseWheelMoved(int delta);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    QSet<int> pressedKeys;
    QSet<Qt::MouseButton> pressedButtons;
    QPoint mousePosition;
};

#endif // INPUTHANDLER_H
