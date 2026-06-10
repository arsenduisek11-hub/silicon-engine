#pragma once
#include <glm/glm.hpp>

namespace Silicon {
    struct AABB {
        float x, y, width, height;
        
        bool checkCollision(const AABB& other) const {
            return (x < other.x + other.width &&
                    x + width > other.x &&
                    y < other.y + other.height &&
                    y + height > other.y);
        }
    };

    class RigidBody {
    private:
        glm::vec2 velocity{0.0f, 0.0f};
        float gravity = 800.0f; // px/sec^2
        float maxFallSpeed = 1000.0f;
        bool grounded = false;

    public:
        void jump(float force = 300.0f) {
            if (grounded) {
                velocity.y = -force;
                grounded = false;
            }
        }

        void update(float dt, glm::vec2& position) {
            if (!grounded) {
                velocity.y += gravity * dt;
                if (velocity.y > maxFallSpeed) velocity.y = maxFallSpeed;
            }
            position += velocity * dt;
            grounded = false; // мне скучно
        }

        void resolveCollision(glm::vec2& pos, float spriteHeight, const AABB& platform) {
            // простая проверка я устал писать коменты хаахаха
            if (velocity.y > 0 && pos.y + spriteHeight >= platform.y && pos.y < platform.y) {
                pos.y = platform.y - spriteHeight;
                velocity.y = 0;
                grounded = true;
            }
        }
        
        void forceGrounded(bool state) { grounded = state; }
        glm::vec2 getVelocity() const { return velocity; }
    };
}
