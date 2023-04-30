#include "openglscene.h"
#include <QMouseEvent>
#include <cmath>
#include "tilemesh.h"
#include "gridmesh.h"
OpenGLScene::OpenGLScene(QWidget *parent) : QOpenGLWidget(parent), currentMode(Mode::Edit)
{
    qDebug() << "Constructor: OpenGLScene";
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
    textureManager.eraseAll();
    doneCurrent();
}
void OpenGLScene::setMode(Mode mode) {
    if(currentMode == mode) return;
    currentMode = mode;
    if(mode == Mode::Edit){
        cameraPosition = {0,0,5};
        for (InteractableTile* tile: interactableTileList) {
            tile->reset();
        }
    }
    else{
        zoomLevel = 2;
        resizeGL(width(), height());
        cameraPosition = {0,4,5};
    }
}
void OpenGLScene::setBackgroundTileSelected(bool isTrue){
    if(box2dWorld != nullptr)
        backgroundTileSelected = isTrue;
    qDebug() << "aa";
}
QVector2D OpenGLScene::worldToScreen(const QVector2D &worldPos) {
    int width = this->width();
    int height = this->height();

    GLint viewport[] = {0, 0, width, height};
    QMatrix4x4 viewMatrix;
    viewMatrix.translate(-cameraPosition);

    QVector4D screenPos4D = (projection * viewMatrix) * QVector4D(worldPos, 0.0, 1.0);
    QVector2D screenPos((screenPos4D.x() / screenPos4D.w() + 1.0) * width / 2.0, (1.0 - screenPos4D.y() / screenPos4D.w()) * height / 2.0);

    return screenPos;
}
QVector2D OpenGLScene::screenToWorld(const QVector2D &screenPos) {
    int width = this->width();
    int height = this->height();

    GLint viewport[] = {0, 0, width, height};
    QVector3D winCoord((2.0 * screenPos.x()) / width - 1.0, 1.0 - (2.0 * screenPos.y()) / height, 0.0);
    QMatrix4x4 viewMatrix;
    viewMatrix.translate(-cameraPosition);

    QVector4D worldPos4D = viewMatrix.inverted() * (projection.inverted() * QVector4D(winCoord, 1.0));
    QVector2D worldPos(worldPos4D.x() / worldPos4D.w(), worldPos4D.y() / worldPos4D.w());
    worldPos.setX(std::floor(worldPos.x()));
    worldPos.setY(std::floor(worldPos.y()));

    return worldPos;
}
void OpenGLScene::addTile(QVector2D pos){
    if (isPositionOccupied(pos)) return ;

    Tile *newTile = tileFactory->createTile(selectedTexture, b2Vec2(pos.x(), pos.y()),!backgroundTileSelected);
    newTile->setPosition(pos);
    tileList.push_back(newTile);
    InteractableTile* interactableTile = dynamic_cast<InteractableTile*>(newTile);
    if (interactableTile != nullptr) {
        interactableTileList.push_back(interactableTile);
        interactableTile->set();
    }
    update();

}
void OpenGLScene::removeTile(QVector2D pos){
    for (auto it = tileList.begin(); it != tileList.end(); ) {
        Tile* tile = *it;
        QVector2D otherpos ={tile->getDrawable()->getPosition().x(),tile->getDrawable()->getPosition().y()};
        if ((pos- otherpos).length() < 0.5) {
            delete tile;
            it = tileList.erase(it);

        } else {
            ++it;
        }
    }
}
void OpenGLScene::mousePressEvent(QMouseEvent *e) {
    if(currentMode == Mode::Play) return;

    QVector2D worldPos = screenToWorld(QVector2D(e->position()));
    if (e->button() == Qt::LeftButton) {
        addTile(worldPos);
    }
    else if (e->button() == Qt::RightButton) {
        removeTile(worldPos);
    }
    else if (e->button() == Qt::MiddleButton){
        mousePressPosition = QVector2D(e->position());
        prevMousePosition = QVector2D(e->position());
    }
}
void OpenGLScene::wheelEvent(QWheelEvent *e) {
    if(currentMode == Mode::Play) return;
    float zoomFactor = 1.1;
    if (-e->angleDelta().y() > 0) {
        zoomLevel *= zoomFactor;
    } else {
        zoomLevel /= zoomFactor;
    }
    resizeGL(width(), height());
}
bool OpenGLScene::isPositionOccupied(QVector2D pos) {
    for (auto& tile : tileList) {
        QVector2D tilePos = {tile->getDrawable()->getPosition().x(), tile->getDrawable()->getPosition().y()};
        if ((tilePos - pos).length() < 0.5) {
            return true;
        }
    }
    return false;
}
void OpenGLScene::mouseMoveEvent(QMouseEvent *e) {
    if(currentMode == Mode::Play) return;

    QVector2D worldPos = screenToWorld(QVector2D(e->position()));
    if (e->buttons() == Qt::LeftButton) {
        addTile(worldPos);
    }
    else if (e->buttons() == Qt::RightButton) {
        removeTile(worldPos);
    }
    else if(e->buttons() == Qt::MiddleButton){
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
    if (playerTile) {
        playerTile->handleKeyEvent(event);
    }
}

void OpenGLScene::keyReleaseEvent(QKeyEvent *event) {
    if (playerTile) {
        playerTile->handleKeyEvent(event);
    }
}
void OpenGLScene::createSolidRectangle(float x1, float y1, float x2, float y2) {
    for (float x = x1; x <= x2; x++) {
        for (float y = y1; y >= y2; y--) {
            Tile *newTile = tileFactory->createTile(selectedTexture, b2Vec2(x, y), !backgroundTileSelected);
            newTile->setPosition({x,y});
            tileList.push_back(newTile);
        }
    }
}
void OpenGLScene::updateScene() {
    float deltaTime = updateTimer->interval() / 1000.0f;


    if (currentMode == Mode::Play) {
        box2dWorld->Step(deltaTime, 6, 4);
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
        // Camera logic
        if (playerTile) {
            QVector2D playerPosition(playerTile->getDrawable()->getPosition().x(), playerTile->getDrawable()->getPosition().y());
            float screenWidth = static_cast<float>(width());
            float screenHeight = static_cast<float>(height());
            QVector2D screenCenter(screenWidth * 0.5f, screenHeight * 0.5f);
            QVector2D playerScreenPosition = worldToScreen(playerPosition);
            float threshold = 50.0f; // Distance from the center to consider moving the camera.

            if (playerScreenPosition.x() > screenCenter.x() + threshold) {
                QVector2D moveAmount(playerScreenPosition.x() - (screenCenter.x() + threshold), 0);
                cameraPosition.setX(cameraPosition.x() + moveAmount.x() * deltaTime);
                if (leftEdgeCollider) {
                    b2Vec2 newPosition(leftBoundary + cameraPosition.x(), 0);
                    leftEdgeCollider->SetTransform(newPosition, 0); // Update the position of the Box2D body
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
    b2BodyDef colliderDef;
    colliderDef.position.Set(leftBoundary+ cameraPosition.x(),0);
    leftEdgeCollider = box2dWorld->CreateBody(&colliderDef);
    b2PolygonShape colliderShape;
    colliderShape.SetAsBox(1, 100 , b2Vec2(0, 0), 0);
    b2FixtureDef colliderFixture;
    colliderFixture.shape = &colliderShape;
    leftEdgeCollider->CreateFixture(&colliderFixture);


    TileMesh *tilemesh = new TileMesh();
    GridMesh *gridmesh = new GridMesh();
    shaderManager.addShader("texture", ":/shaders/vtexture.glsl", ":/shaders/ftexture.glsl");

    tileFactory = new TileFactory(box2dWorld,&textureManager,&shaderManager);
    grid = new Drawable(shaderManager.getShader("texture"), textureManager.getTexture("white"), gridmesh);

    Tile* createdTile = tileFactory->createTile("mario", b2Vec2(0, 0));
    playerTile = dynamic_cast<PlayerTile*>(createdTile);
    tileList.push_back(playerTile);
    interactableTileList.push_back(playerTile);

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &OpenGLScene::updateScene);
    updateTimer->start(16); // approximate 60fps updates
    setSelectedTexture("brownBlock1");
    createSolidRectangle(-10, -1, 40, -8);
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

    leftBoundary = -orthoSize * aspect ;
}

void OpenGLScene::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view;
    view.translate(-cameraPosition);

    if (currentMode == Mode::Edit)
        grid->draw(projection,view);
    for (auto &obj : tileList)
    {
        obj->draw(projection, view);
    }
}
