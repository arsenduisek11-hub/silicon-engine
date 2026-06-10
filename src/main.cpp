#define STB_IMAGE_IMPLEMENTATION // определим имбовую позку
#include "Core/Engine.hpp"
#include "Physics/Physics.hpp"

class MyScene : public Silicon::Scene {
public:
    Silicon::Sprite player;
    Silicon::RigidBody physics;
    Silicon::AABB platform;

    void onStart(Silicon::Renderer& r) override {
        player.load("assets/player.png"); // Положи рандомную лаки картиночку в крутую папку  в assets/
        player.position = {100.0f, 100.0f};
        player.size = {50.0f, 50.0f};

        // Платформа вроде нормально или нет хз кароче
        platform = {50.0f, 400.0f, 700.0f, 50.0f}; 
    }

    void onUpdate(float dt) override {
        // кароче это вод
        float speed = 200.0f;
        if (Silicon::Input::isKeyDown(GLFW_KEY_RIGHT)) player.position.x += speed * dt;
        if (Silicon::Input::isKeyDown(GLFW_KEY_LEFT)) player.position.x -= speed * dt;
        if (Silicon::Input::isKeyDown(GLFW_KEY_SPACE)) physics.jump(400.0f);

        // 2. Физика или математика или геометриця кароче физика
        physics.update(dt, player.position);

        // 3. Коллизии или нет или да кароче да
        physics.resolveCollision(player.position, player.size.y, platform);
    }

    void onDraw(Silicon::Renderer& r) override {
        r.draw(player);
    }
};

int main() {
    Silicon::Engine engine(800, 600, "Silicon Engine v0.1");
    engine.run<MyScene>();
    return 0;
}
