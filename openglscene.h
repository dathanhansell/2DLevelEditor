#ifndef MAINWIDGET_H
#define MAINWIDGET_H


#include "shadermanager.h"
#include "drawable.h"
#include "playertile.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTimer>
#include "camera.h"
#include "inputhandler.h"


class OpenGLScene : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    using QOpenGLWidget::QOpenGLWidget;
    explicit OpenGLScene(QWidget *parent = nullptr);
    //void setSelectedBlockType(int index);
    ~OpenGLScene();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;


    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    QVector3D screenToWorld(const QVector2D &screenPos);
private slots:
    void updateScene();
private:
    ShaderManager shaderManager;
    QVector2D prevMousePosition;
    std::vector<Drawable *> drawableObjects;
    b2World *box2dWorld;
    std::vector<Tile *> tileList;
    QMatrix4x4 projection;
    QVector3D cameraPosition;
    QVector2D mousePressPosition;
    QTimer *updateTimer;
    PlayerTile *playerTile;
    Camera camera;
    InputHandler inputHandler;
};

#endif // MAINWIDGET_H
