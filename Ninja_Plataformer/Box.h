#ifndef BOX_H
#define BOX_H

#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include <Bengine/Vertex.h>
#include <Bengine/GLTexture.h>
#include <Bengine/SpriteBatch.h>

class Box
{
    public:
        Box();
        virtual ~Box();

        void init(b2World* world,
                  const glm::vec2& position,
                  const glm::vec2& dimensions,
                  Bengine::GLTexture texture,
                  const Bengine::ColorRGBA8& color,
                  bool fixedRotation,
                  bool isDynamic,
                  float angle = 0.0f,
                  glm::vec4 uvRectangle = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        void destroy(b2World* world);

        void draw(Bengine::SpriteBatch& spriteBatch);

        bool isDynamic() const { return m_body->GetType() == b2_dynamicBody; }

        // Test if a point is inside the box
        bool testPoint(float x, float y) const { return m_fixture->TestPoint(b2Vec2(x, y)); }

        b2Body* getBody()          const { return m_body; }
        b2Fixture* getFixture()       const { return m_fixture; }
        const glm::vec2& getDimensions()    const { return m_dimensions; }
        glm::vec2                  getPosition()      const { return glm::vec2(m_body->GetPosition().x, m_body->GetPosition().y); }
        glm::vec4                  getUvRect()        const { return m_uvRect; }
        const Bengine::ColorRGBA8& getColor()         const { return m_color; }
        float                      getAngle()         const { return m_body->GetAngle(); }
        const Bengine::GLTexture& getTexture()       const { return m_texture; }
        const bool& getFixedRotation() const { return m_fixedRotation; }
        const bool& getIsDynamic()     const { return m_isDynamic; }

    protected:

    private:
        glm::vec4 m_uvRect;
        glm::vec2 m_dimensions;
        bool m_fixedRotation;
        bool m_isDynamic;
        b2Body* m_body = nullptr;
        b2Fixture* m_fixture = nullptr;
        

        Bengine::ColorRGBA8 m_color;
        Bengine::GLTexture m_texture;
};

#endif // BOX_H
