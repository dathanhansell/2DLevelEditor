#ifndef DEBUGGRAPHICS_H
#define DEBUGGRAPHICS_H
#include "drawable.h"
#include <QVector2D>
#include <vector>
#include "texturemanager.h"
#include "shadermanager.h"

class DebugGraphics {
public:
    DebugGraphics(TextureManager* textureManager_, ShaderManager* shaderManager_);

    void setMatrices(const QMatrix4x4& projection_, const QMatrix4x4& view_);
    void drawPoint(QVector2D position);
    void drawLine(QVector2D startPosition, QVector2D endPosition);
    void drawSquare(QVector2D position, QVector2D size);

private:
    QMatrix4x4 projection;
    QMatrix4x4 view;
    Drawable* createDrawable();
    std::vector<Drawable*> points;
    std::vector<Drawable*> lines;
    std::vector<Drawable*> squares;
    TextureManager* textureManager;
    ShaderManager* shaderManager;
};

#endif // DEBUGGRAPHICS_H
