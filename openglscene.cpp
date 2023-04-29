#include "openglscene.h"
#include <QMouseEvent>
#include <cmath>
#include "tilemesh.h"
#include "gridmesh.h"
OpenGLScene::OpenGLScene(QWidget *parent) : QOpenGLWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    installEventFilter(this); // Install the event filter
}
OpenGLScene::~OpenGLScene() {
    makeCurrent();
    setFocusPolicy(Qt::StrongFocus);
    for (auto& obj : drawableObjects) {
        delete obj;
    }
    for (auto& tile : tileList) {
        delete tile;
    }
    delete box2dWorld;
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
        Drawable *newTileDrawable = new Drawable(shaderManager.getShader("texture"), new QOpenGLTexture(QImage(":/textures/cube.png").mirrored()), tilemesh);
        b2Vec2 tilePos(worldPos.x(), worldPos.y());
        b2Vec2 tileSize(1, 1);
        Tile *newTile = new Tile(newTileDrawable, box2dWorld, tilePos, tileSize);
        tileList.push_back(newTile);
        drawableObjects.push_back(newTile->getDrawable());
        update();
        qDebug() << "new tile at " << worldPos;
    }
    else if(e->button() == Qt::RightButton){

        mousePressPosition = QVector2D(e->position());
        prevMousePosition = QVector2D(e->position());
    }

}
bool OpenGLScene::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        keyPressEvent(static_cast<QKeyEvent *>(event));
        return true;
    }
    if (event->type() == QEvent::KeyRelease) {
        keyReleaseEvent(static_cast<QKeyEvent *>(event));
        return true;
    }
    return QOpenGLWidget::eventFilter(obj, event);
}
void OpenGLScene::keyPressEvent(QKeyEvent *event) {
    qDebug() << "trying keypress";
    if (playerTile) {
        qDebug() << "trying keypress";
        playerTile->handleKeyEvent(event);
    }
}

void OpenGLScene::keyReleaseEvent(QKeyEvent *event) {
    if (playerTile) {
        playerTile->handleKeyEvent(event);
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
void OpenGLScene::updateScene() {
    // Update the tiles
    float deltaTime = updateTimer->interval() / 1000.0f;
    box2dWorld->Step(deltaTime, 6, 2);
    for (Tile* tile: tileList) {
        qDebug() << "tile update";
        tile->update(deltaTime);
    }
    // Check for collisions
    for (Tile* tile: tileList) {
        if (InteractableTile* itile = dynamic_cast<InteractableTile*>(tile)) {
            for (Tile* other: tileList) {
                if (tile != other && tile->isCollidingWith(other)) {
                    itile->onCollision(other);
                }
            }
        }
    }

    update();
}
void OpenGLScene::initializeGL() {
    cameraPosition = QVector3D(0.0, 0.0, 5.0);
    initializeOpenGLFunctions();
    glClearColor(0, .4f, .9f, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    box2dWorld = new b2World(b2Vec2(0, -4));
    TileMesh *tilemesh = new TileMesh();
    GridMesh *gridmesh = new GridMesh();
    shaderManager.addShader("texture", ":/shaders/vtexture.glsl", ":/shaders/ftexture.glsl");
    shaderManager.addShader("whiteShader", ":/shaders/vwhite.glsl", ":/shaders/fwhite.glsl");
    //Drawable *tileObject = new Drawable(shaderManager.getShader("texture"), new QOpenGLTexture(QImage(":/textures/cube.png").mirrored()), tilemesh);
    Drawable *gridObject = new Drawable(shaderManager.getShader("texture"), new QOpenGLTexture(QImage(":/textures/white.png").mirrored()), gridmesh);

    //drawableObjects.push_back(tileObject);
    drawableObjects.push_back(gridObject);
    Drawable *playerDrawable = new Drawable(shaderManager.getShader("texture"), new QOpenGLTexture(QImage(":/textures/cube.png").mirrored()), tilemesh);
    playerTile = new PlayerTile(playerDrawable, box2dWorld, b2Vec2(0, 0), b2Vec2(1, 1));
    drawableObjects.push_back(playerTile->getDrawable());
    tileList.push_back(playerTile);
    updateTimer = new QTimer(this);
    qDebug() << "timer";
    connect(updateTimer, &QTimer::timeout, this, &OpenGLScene::updateScene);
    updateTimer->start(16); // approximate 60fps updates
    qDebug()<<updateTimer->isActive();
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

    // Draw drawable objects
    for (auto &obj : drawableObjects)
    {
        obj->draw(projection, view);
    }
}
