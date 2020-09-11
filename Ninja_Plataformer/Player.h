#ifndef PLAYER_H
#define PLAYER_H

#include "Capsule.h"
#include <Bengine/SpriteBatch.h>
#include <Bengine/TileSheet.h>
#include <Bengine/InputManager.h>

enum class PlayerMoveState{ STANDING, RUNNING, PUNCHING, IN_AIR };

class Player
{
    public:
        Player();
        virtual ~Player();

        void init(b2World* world,
                  const glm::vec2& position,
                  const glm::vec2& drawDims,
                  const glm::vec2& collitionDims,
                  const Bengine::ColorRGBA8& color);
        void destroy(b2World* world);

        void draw(Bengine::SpriteBatch& spriteBatch, float deltaTime = 1.0f); ///< Delta time used for animation
        void drawDebug(Bengine::DebugRenderer& debugRenderer);

        void update(Bengine::InputManager& inputManager, float deltaTime);///< Delta time not used here but in m_world->step

        const Capsule& getCapsule() const { return m_capsule; }
        glm::vec2 getPosition() const { return glm::vec2(m_capsule.getBody()->GetPosition().x, m_capsule.getBody()->GetPosition().y); }
        bool getIsLightOn() const { return m_isLightOn; }
        const glm::vec2& getDrawDims() const { return m_drawDims; }
        const glm::vec2& getCollisionDims() const { return m_collisionDims; }
        const Bengine::ColorRGBA8& getColor() const { return m_color; }

    protected:

    private:
        Capsule m_capsule;
        Bengine::TileSheet m_texture;
        Bengine::ColorRGBA8 m_color;
        glm::vec2 m_drawDims;
        glm::vec2 m_collisionDims;
        PlayerMoveState m_moveState = PlayerMoveState::STANDING;
        int m_direction = 1; ///< 1 or -1
        bool m_onGround = false;
        bool m_isLightOn = false;
        bool m_isPunching = false;
        float m_animTime = 0.0f; ///< Used for animation
};

#endif // PLAYER_H
