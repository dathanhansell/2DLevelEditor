#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>

class Camera
{
public:
    Camera();

    QVector3D getPosition() const;
    void setPosition(const QVector3D& position);
    void move(const QVector3D& delta);
    void zoom(float factor);
    QMatrix4x4 getViewMatrix() const;

private:
    QVector3D position;
    float zoomFactor;
};

#endif // CAMERA_H
