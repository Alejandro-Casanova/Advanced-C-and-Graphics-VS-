#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H

#include <vector>
#include <GLEW/glew.h>
#include <glm/glm.hpp>
#include "Vertex.h"

namespace Bengine{

enum class GlyphSortType{
    NONE,
    FRONT_TO_BACK,
    BACK_TO_FRONT,
    TEXTURE
};

class Glyph{
public:
    Glyph() {}
    Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& Color);
    Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& Color, float angle);


    GLuint texture = 0;
    float depth = 0;

    Vertex topLeft;
    Vertex bottomLeft;
    Vertex topRight;
    Vertex bottomRight;
private:
    glm::vec2 rotatePoint(glm::vec2 point, float angle);

};

class RenderBatch{
public:
    RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) :
        offset(Offset),
        numVertices(NumVertices),
        texture(Texture){}
    GLuint offset;
    GLuint numVertices;
    GLuint texture;
};

class SpriteBatch{
public:
    SpriteBatch();
    virtual ~SpriteBatch();

    void init();

    void dispose();

    void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
    void end();

    void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);//Destination Rectangle: 2 pocition coordinates and 2 dimension coordinates
    void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle);//Angle in radians
    void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& directionVect);

    void renderBatch();


private:
    void createRenderBatches();
    void createVertexArray();
    void sortGlyphs();

    static bool compareFrontToBack(Glyph* a, Glyph* b);
    static bool compareBackToFront(Glyph* a, Glyph* b);
    static bool compareTexture(Glyph* a, Glyph* b);

    GLuint m_vbo = 0;
    GLuint m_vao = 0;

    GlyphSortType m_sortType = GlyphSortType::TEXTURE;

    std::vector<Glyph*> m_glyphPointers;
    std::vector<Glyph> m_glyphs;
    std::vector<RenderBatch> m_renderBatches;
};

}

#endif // SPRITEBATCH_H
