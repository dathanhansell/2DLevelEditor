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
#include "contactlistener.h"
//#include "camera.h"
#include "debuggraphics.h"
#include "texturemanager.h"
#include "tilefactory.h"
enum class Mode { Edit, Play };

class OpenGLScene : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    using QOpenGLWidget::QOpenGLWidget;
    explicit OpenGLScene(QWidget *parent = nullptr);
    void setSelectedTexture(const QString& textureName);
    ~OpenGLScene();
    void setBackgroundTileSelected(bool isTrue);
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;
public slots:
    void setMode(Mode mode);
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    QVector2D screenToWorld(const QVector2D &screenPos);
    QVector2D worldToScreen(const QVector2D &worldPos);
private slots:
    void updateScene();
private:
    DebugGraphics* debugGraphics;
    ContactListener* contactListener;
    float leftBoundary = 0;
    b2Body* leftEdgeCollider;
    bool isPositionOccupied(QVector2D pos);
    Mode currentMode;
    void createSolidRectangle(float x1, float y1, float x2, float y2);
    void addTile(QVector2D pos);
    void removeTile(QVector2D pos);
    bool backgroundTileSelected = false;
    float zoomLevel =1;
    ShaderManager shaderManager;
    TextureManager textureManager;
    QVector2D prevMousePosition;
    Drawable* grid;
    QString selectedTexture;
    b2World *box2dWorld;
    std::vector<Tile *> tileList;
    std::vector<InteractableTile *> interactableTileList;
    QMatrix4x4 projection;
    QVector3D cameraPosition;
    QVector2D mousePressPosition;
    QTimer *updateTimer;
    PlayerTile *playerTile;
    TileFactory *tileFactory;
};
#endif // MAINWIDGET_H
