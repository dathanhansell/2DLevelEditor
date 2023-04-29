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
    for (auto& tile : tileList) {
        delete tile;
    }
    delete box2dWorld;
    delete tileFactory;
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
    QVector3D worldPos = screenToWorld(QVector2D(e->position()));
    if (e->button() == Qt::LeftButton) {
        // Add tile
        Tile *newTile = tileFactory->createTile(selectedTexture, b2Vec2(worldPos.x(), worldPos.y()));
        tileList.push_back(newTile);

        update();
    }
    else if (e->button() == Qt::RightButton) {
        // Remove tile
        for (auto it = tileList.begin(); it != tileList.end(); ) {
            Tile* tile = *it;
            if ((tile->getDrawable()->getPosition() - worldPos).length() < 0.5) {
                delete tile;
                it = tileList.erase(it);
            } else {
                ++it;
            }
        }
    }
    else if (e->button() == Qt::MiddleButton){
        mousePressPosition = QVector2D(e->position());
        prevMousePosition = QVector2D(e->position());
    }
}
void OpenGLScene::wheelEvent(QWheelEvent *e) {
    float zoomFactor = 1.1;
    if (-e->angleDelta().y() > 0) {
        zoomLevel *= zoomFactor;
    } else {
        zoomLevel /= zoomFactor;
    }
    resizeGL(width(), height());
}

void OpenGLScene::mouseMoveEvent(QMouseEvent *e) {
    if(e->buttons() == Qt::MiddleButton){
        QVector2D diff = QVector2D(e->position()) - prevMousePosition;
        float speedModifier = 1.0f * zoomLevel; // This line is added to modify the speed based on zoomLevel
        cameraPosition += QVector3D(-diff.x() * speedModifier / 100.0, diff.y() * speedModifier / 100.0, 0.0);
        prevMousePosition = QVector2D(e->position());
        update();
    }
}
void OpenGLScene::setSelectedTexture(const QString & textureName)
{
    selectedTexture = textureName;
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

void OpenGLScene::updateScene() {
    float deltaTime = updateTimer->interval() / 1000.0f;
    box2dWorld->Step(deltaTime, 6, 2);
    for (Tile* tile: tileList) {
        tile->update(deltaTime);
    }
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
    glClearColor(0.36f, 0.58f ,.98f, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    box2dWorld = new b2World(b2Vec2(0, -9.81));
    TileMesh *tilemesh = new TileMesh();
    GridMesh *gridmesh = new GridMesh();

    setSelectedTexture("tile0");

    shaderManager.addShader("texture", ":/shaders/vtexture.glsl", ":/shaders/ftexture.glsl");
    shaderManager.addShader("whiteShader", ":/shaders/vwhite.glsl", ":/shaders/fwhite.glsl");

    tileFactory = new TileFactory(box2dWorld,&textureManager,&shaderManager);
    setSelectedTexture("brownBlock1");
    //Drawable *tileObject = new Drawable(shaderManager.getShader("texture"), new QOpenGLTexture(QImage(":/textures/cube.png").mirrored()), tilemesh);

    grid = new Drawable(shaderManager.getShader("texture"), textureManager.getTexture("white"), gridmesh);

    playerTile = tileFactory->createPlayerTile("mario",b2Vec2(0,0));
    tileList.push_back(playerTile);
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &OpenGLScene::updateScene);
    updateTimer->start(16); // approximate 60fps updates
}

void OpenGLScene::resizeGL(int w, int h) {
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    qreal orthoSize = 5.0 * zoomLevel;
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
    grid->draw(projection,view);
    for (auto &obj : tileList)
    {
        obj->draw(projection, view);
    }
}
