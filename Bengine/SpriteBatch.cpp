#include "pch.h"

#include "SpriteBatch.h"

#include <algorithm>

namespace Bengine{

Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& Color) :
    texture(Texture), depth(Depth){

    topLeft.color = Color;
    topLeft.setPosition(destRect.x, destRect.y + destRect.w);
    topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

    bottomLeft.color = Color;
    bottomLeft.setPosition(destRect.x, destRect.y);
    bottomLeft.setUV(uvRect.x, uvRect.y);

    bottomRight.color = Color;
    bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
    bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

    topRight.color = Color;
    topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
    topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
}

Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& Color, float angle) :
    texture(Texture), depth(Depth){

    ///Get vertices centered at origin
    glm::vec2 halfDims( destRect.z / 2.0f, destRect.w / 2.0f);
    glm::vec2 tl(-halfDims.x, halfDims.y);
    glm::vec2 bl(-halfDims.x, -halfDims.y);
    glm::vec2 br(halfDims.x, -halfDims.y);
    glm::vec2 tr(halfDims.x, halfDims.y);

    ///Rotate points
    tl = rotatePoint(tl, angle) + halfDims;
    bl = rotatePoint(bl, angle) + halfDims;
    br = rotatePoint(br, angle) + halfDims;
    tr = rotatePoint(tr, angle) + halfDims;

    topLeft.color = Color;
    topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y);
    topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

    bottomLeft.color = Color;
    bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y);
    bottomLeft.setUV(uvRect.x, uvRect.y);

    bottomRight.color = Color;
    bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y);
    bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

    topRight.color = Color;
    topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y);
    topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
}

glm::vec2 Glyph::rotatePoint(glm::vec2 point, float angle){
    glm::vec2 newPoint;
    newPoint.x = point.x * cos(angle) - point.y * sin(angle);
    newPoint.y = point.x * sin(angle) + point.y * cos(angle);
    return newPoint;
}

SpriteBatch::SpriteBatch()
{
    //ctor
}

SpriteBatch::~SpriteBatch()
{
//    for(auto it = _glyphs.begin(); it != _glyphs.end(); it++){
//        delete (*it);
//    }
    dispose();
}

void SpriteBatch::init(){
    createVertexArray();
}

void SpriteBatch::dispose() {
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
}

void SpriteBatch::begin(GlyphSortType sortType /*= GlyphSortType::TEXTURE*/){
    m_sortType = sortType;
    m_renderBatches.clear();

//    for(auto it = _glyphs.begin(); it != _glyphs.end(); it++){
//        delete (*it);
//    }
//    for(int i = 0; i < _glyphs.size(); i++){
//        delete _glyphs[i];
//    }
    m_glyphs.clear();
}

void SpriteBatch::end(){

    m_glyphPointers.resize(m_glyphs.size());
    for(size_t i = 0; i < m_glyphPointers.size(); i++){
        m_glyphPointers[i] = &m_glyphs[i];
    }

    sortGlyphs();
    createRenderBatches();
}

//Destination Rectangle: 2 pocition coordinates and 2 dimension coordinates
void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color){
    m_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle){//Angle in radians
    m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
}

void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& directionVect){
    const glm::vec2 right(1.0f, 0.0f);
    float angle = acos(glm::dot(right, directionVect));
    if(directionVect.y < 0){///< The "acos" function only returns positive angles
        angle = -angle;
    }

    m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
}

void SpriteBatch::renderBatch(){

    glBindVertexArray(m_vao);

//    for(int i = 0; i < _renderBatches.size(); i++){
//        glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
//
//        glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
//    }
    for(auto& it : m_renderBatches){
        glBindTexture(GL_TEXTURE_2D, it.texture);

        glDrawArrays(GL_TRIANGLES, it.offset, it.numVertices);
    }


    glBindVertexArray(0);
}

void SpriteBatch::createRenderBatches(){
    std::vector<Vertex> vertices;
    vertices.resize(m_glyphs.size() * 6);

    if(m_glyphs.empty()){
        return;
    }

    int offset = 0;
    int cv = 0;//Current vertex
    m_renderBatches.emplace_back(offset, 6, m_glyphPointers[0]->texture);
    vertices[cv++] = m_glyphPointers[0]->topLeft;
    vertices[cv++] = m_glyphPointers[0]->bottomLeft;
    vertices[cv++] = m_glyphPointers[0]->bottomRight;
    vertices[cv++] = m_glyphPointers[0]->bottomRight;
    vertices[cv++] = m_glyphPointers[0]->topRight;
    vertices[cv++] = m_glyphPointers[0]->topLeft;
    offset += 6;

    for(size_t cg = 1; cg < m_glyphPointers.size(); cg++){
        if(m_glyphPointers[cg]->texture != m_glyphPointers[cg - 1]->texture){
            m_renderBatches.emplace_back(offset, 6, m_glyphPointers[cg]->texture);
        }else{
            m_renderBatches.back().numVertices += 6;
        }

        vertices[cv++] = m_glyphPointers[cg]->topLeft;
        vertices[cv++] = m_glyphPointers[cg]->bottomLeft;
        vertices[cv++] = m_glyphPointers[cg]->bottomRight;
        vertices[cv++] = m_glyphPointers[cg]->bottomRight;
        vertices[cv++] = m_glyphPointers[cg]->topRight;
        vertices[cv++] = m_glyphPointers[cg]->topLeft;
        offset += 6;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    //Orphan the buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    //Upload the data
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    //Unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void SpriteBatch::createVertexArray(){

    if(m_vao == 0){
        //Generate vertex array object
        glGenVertexArrays(1, &m_vao);
    }
    glBindVertexArray(m_vao);

    if(m_vbo == 0){
        glGenBuffers(1, &m_vbo);
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    //Enable attribute arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    //This is the position attribute pointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    //This is the color attribute pointer
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    //This is the UV attribute pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    //Disable all vertex attributes and unbind vbo and vao
    glBindVertexArray(0);


}

void SpriteBatch::sortGlyphs(){
    switch(m_sortType){
    case GlyphSortType::BACK_TO_FRONT:
        std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareBackToFront);
        break;
    case GlyphSortType::FRONT_TO_BACK:
        std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareFrontToBack);
        break;
    case GlyphSortType::TEXTURE:
        std::stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareTexture);
        break;
    default:
        break;
    }

}

bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b){
    return (a->depth < b->depth);
}

bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b){
    return (a->depth > b->depth);
}

bool SpriteBatch::compareTexture(Glyph* a, Glyph* b){
    return (a->texture < b->texture);
}

}
