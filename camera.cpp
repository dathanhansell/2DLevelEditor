#include "camera.h"

Camera::Camera() : position(0.0, 0.0, 5.0), zoomFactor(1.0f)
{
}

QVector3D Camera::getPosition() const
{
    return position;
}

void Camera::setPosition(const QVector3D& newPosition)
{
    position = newPosition;
}

void Camera::move(const QVector3D& delta)
{
    position += delta;
}

void Camera::zoom(float factor)
{
    zoomFactor *= factor;
}

QMatrix4x4 Camera::getViewMatrix() const
{
    QMatrix4x4 view;
    view.translate(-position);
    view.scale(zoomFactor);
    return view;
}
