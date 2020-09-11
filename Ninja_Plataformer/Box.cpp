#include "Box.h"

Box::Box()
{
    //ctor
}

Box::~Box()
{
    //dtor
}

void Box::init(b2World* world,
               const glm::vec2& position,
               const glm::vec2& dimensions,
               Bengine::GLTexture texture,
               const Bengine::ColorRGBA8& color,
               bool fixedRotation,
               bool isDynamic,
               float angle, /* = 0.0f */
               glm::vec4 uvRectangle /*= glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)*/){

    m_uvRect = uvRectangle;
    m_dimensions = dimensions;
    m_color = color;
    m_texture = texture;
    m_fixedRotation = fixedRotation;
    m_isDynamic = isDynamic;
    

    // Make the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    if (isDynamic) {
        bodyDef.type = b2_dynamicBody;
    }
    else {
        bodyDef.type = b2_staticBody;
    }
    bodyDef.position.Set(position.x, position.y);
    bodyDef.fixedRotation = fixedRotation;
    bodyDef.angle = angle;
    m_body = world->CreateBody(&bodyDef);

    ///Make shape
    b2PolygonShape boxShape;
    boxShape.SetAsBox(dimensions.x / 2.0f, dimensions.y / 2.0f);

    ///Make fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    m_fixture = m_body->CreateFixture(&fixtureDef);


}

void Box::destroy(b2World* world)
{
    world->DestroyBody(m_body);
}

void Box::draw(Bengine::SpriteBatch& spriteBatch){
    glm::vec4 destRect;
    ///Spritebatch draw function needs the origin of the bodies to be on their corner (not centered)
    destRect.x = m_body->GetPosition().x - m_dimensions.x / 2.0f;
    destRect.y = m_body->GetPosition().y - m_dimensions.y / 2.0f;
    destRect.z = m_dimensions.x;
    destRect.w = m_dimensions.y;

    ///Draw boxes
    spriteBatch.draw(destRect,
                       m_uvRect,
                       m_texture.id, 0.0f,
                       m_color,
                       m_body->GetAngle());
}
