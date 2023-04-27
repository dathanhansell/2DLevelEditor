#include "openglscene.h"
#include <QMouseEvent>
#include <cmath>
#include "tilemesh.h"

OpenGLScene::~OpenGLScene() {
    makeCurrent();
    delete texture;
    for (auto& obj : drawableObjects) {
        delete obj;
    }
    delete geometries;
    doneCurrent();
}

void OpenGLScene::mousePressEvent(QMouseEvent *e) {
    mousePressPosition = QVector2D(e->position());
    prevMousePosition = QVector2D(e->position());
}

void OpenGLScene::mouseMoveEvent(QMouseEvent *e) {
    QVector2D diff = QVector2D(e->position()) - prevMousePosition;
    cameraPosition += QVector3D(-diff.x() / 100.0, diff.y() / 100.0, 0.0);
    prevMousePosition = QVector2D(e->position());
    update();
}

void OpenGLScene::initializeGL() {
    cameraPosition = QVector3D(0.0, 0.0, 5.0);
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    initShaders();
    initTextures();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    geometries = new GeometryEngine;
    TileMesh *tilemesh = new TileMesh();

    Drawable *tileObject = new Drawable(shaderManager.getShader("texture"), texture, tilemesh);
    //Drawable *gridObject = new Drawable(shaderManager.getShader("whiteShader"), nullptr, geometries);

    drawableObjects.push_back(tileObject);
    //drawableObjects.push_back(gridObject);
}

void OpenGLScene::initShaders() {
    shaderManager.addShader("texture", ":/shaders/vtexture.glsl", ":/shaders/ftexture.glsl");
    shaderManager.addShader("whiteShader", ":/shaders/vwhite.glsl", ":/shaders/fwhite.glsl");
}

void OpenGLScene::initTextures() {
    texture = new QOpenGLTexture(QImage(":/textures/cube.png").mirrored());
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::Repeat);
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
