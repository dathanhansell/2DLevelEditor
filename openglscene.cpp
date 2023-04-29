#include "openglscene.h"
#include <QMouseEvent>
#include <cmath>
#include "tilemesh.h"
#include "gridmesh.h"

OpenGLScene::~OpenGLScene() {
    makeCurrent();
    for (auto& obj : drawableObjects) {
        delete obj;
    }
    doneCurrent();
}
QVector3D OpenGLScene::screenToWorld(const QVector2D &screenPos) {
    int width = this->width();
    int height = this->height();

    GLint viewport[] = {0, 0, width, height};
    QVector3D winCoord((2.0 * screenPos.x()) / width - 1.0, 1.0 - (2.0 * screenPos.y()) / height, 0.0);
    QMatrix4x4 viewMatrix;
    viewMatrix.translate(-cameraPosition);

    QVector4D worldPos4D = viewMatrix.inverted() * (projection.inverted() * QVector4D(winCoord, 1.0));
    QVector3D worldPos(worldPos4D.x() / worldPos4D.w(), worldPos4D.y() / worldPos4D.w(), 0.0);
    worldPos.setX(std::floor(worldPos.x()));
    worldPos.setY(std::floor(worldPos.y()));

    return worldPos;
}

void OpenGLScene::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        QVector3D worldPos = screenToWorld(QVector2D(e->position()));
        TileMesh *tilemesh = new TileMesh();
        Drawable *newTile = new Drawable(shaderManager.getShader("texture"), new QOpenGLTexture(QImage(":/textures/cube.png").mirrored()), tilemesh);
        newTile->setPosition(worldPos);
        drawableObjects.push_back(newTile);
        update();
        qDebug() << "new tile at " <<worldPos;
    }
    else if(e->button() == Qt::RightButton){

        mousePressPosition = QVector2D(e->position());
        prevMousePosition = QVector2D(e->position());
    }

}

void OpenGLScene::mouseMoveEvent(QMouseEvent *e) {
    if(e->buttons() == Qt::RightButton){

        QVector2D diff = QVector2D(e->position()) - prevMousePosition;
        cameraPosition += QVector3D(-diff.x() / 100.0, diff.y() / 100.0, 0.0);
        prevMousePosition = QVector2D(e->position());
        update();
    }
}

void OpenGLScene::initializeGL() {
    cameraPosition = QVector3D(0.0, 0.0, 5.0);
    initializeOpenGLFunctions();
    glClearColor(0, .4f, .9f, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    TileMesh *tilemesh = new TileMesh();
    GridMesh *gridmesh = new GridMesh();
    shaderManager.addShader("texture", ":/shaders/vtexture.glsl", ":/shaders/ftexture.glsl");
    shaderManager.addShader("whiteShader", ":/shaders/vwhite.glsl", ":/shaders/fwhite.glsl");
    //Drawable *tileObject = new Drawable(shaderManager.getShader("texture"), new QOpenGLTexture(QImage(":/textures/cube.png").mirrored()), tilemesh);
    Drawable *gridObject = new Drawable(shaderManager.getShader("texture"), new QOpenGLTexture(QImage(":/textures/white.png").mirrored()), gridmesh);

    //drawableObjects.push_back(tileObject);
    drawableObjects.push_back(gridObject);
}

void OpenGLScene::resizeGL(int w, int h) {
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    qreal orthoSize = 5.0;
    qreal left = -orthoSize * aspect;
    qreal right = orthoSize * aspect;
    qreal bottom = -orthoSize;
    qreal top = orthoSize;
    qreal zNear = 3.0;
    qreal zFar = 7.0;

    projection.setToIdentity();
    projection.ortho(left, right, bottom, top, zNear, zFar);
}

void OpenGLScene::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view;
    view.translate(-cameraPosition);

    for (auto &obj : drawableObjects)
    {
        obj->draw(projection, view);
    }
}
