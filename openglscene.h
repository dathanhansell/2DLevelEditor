#ifndef MAINWIDGET_H
#define MAINWIDGET_H


#include "shadermanager.h"
#include "drawable.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class GeometryEngine;

class OpenGLScene : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    using QOpenGLWidget::QOpenGLWidget;
    ~OpenGLScene();

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    QVector3D screenToWorld(const QVector2D &screenPos);

private:
    ShaderManager shaderManager;
    QVector2D prevMousePosition;
    std::vector<Drawable *> drawableObjects;

    QMatrix4x4 projection;
    QVector3D cameraPosition;
    QVector2D mousePressPosition;
};

#endif // MAINWIDGET_H
