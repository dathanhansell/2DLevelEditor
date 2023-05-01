// debuggraphics.cpp
#include "debuggraphics.h"
#include "tilemesh.h"

DebugGraphics::DebugGraphics(TextureManager* textureManager_, ShaderManager* shaderManager_) :
    textureManager(textureManager_), shaderManager(shaderManager_) {}

void DebugGraphics::setMatrices(const QMatrix4x4& projection_, const QMatrix4x4& view_) {
    projection = projection_;
    view = view_;
}

void DebugGraphics::drawPoint(QVector2D position ) {
    Drawable* point = createDrawable();
    point->setPosition(QVector2D(position.x(), position.y()));
    point->draw(projection, view);
    points.push_back(point);
}

void DebugGraphics::drawLine(QVector2D startPosition, QVector2D endPosition) {
    Drawable* line = createDrawable();
    line->setPosition(QVector2D((startPosition.x() + endPosition.x()) / 2.0, (startPosition.y() + endPosition.y()) / 2.0));
    line->setScale(QVector2D((endPosition - startPosition).length(), 0.03));
    line->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0, 0.0, 1.0), qRadiansToDegrees(std::atan2(endPosition.y() - startPosition.y(), endPosition.x() - startPosition.x()))));
    line->draw(projection, view);
    lines.push_back(line);
}

void DebugGraphics::drawSquare(QVector2D position, QVector2D size) {
    Drawable* square = createDrawable();
    square->setPosition(QVector2D(position.x(), position.y()));
    square->setScale(size);
    square->draw(projection, view);
    squares.push_back(square);
}

Drawable* DebugGraphics::createDrawable() {
    TileMesh* mesh = new TileMesh();
    QOpenGLTexture* texture = textureManager->getTexture("white");
    QOpenGLShaderProgram* shader = shaderManager->getShader("texture");
    Drawable* drawable = new Drawable(shader, texture, mesh);
    return drawable;
}
