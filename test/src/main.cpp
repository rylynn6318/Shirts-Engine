#include <iostream>
#include <component/TextureComponent.h>
#include <random>
#include "component/Component.h"
#include "entity/EntityDB.h"
#include "render/Renderer.h"
#include "core/Game.h"

struct TempComponent : public se::Component<TempComponent> {};

int main() {
    se::Renderer renderer;
    renderer.init(1200, 800);

    auto pepe_texture = se::loadTexture("../resource/image/pepe_thumsup.png");

    se::EntityDB db;
    for (int i = 0; i < 10; ++i) {
        auto e = db.createEntity();
        db.addComponent(e, se::TransformComponent{{(float)(i * 10), 0.0f, 0.0f}, {}, {}}, se::TextureComponent{pepe_texture});
        if (i == 5)
            db.addComponent(e, TempComponent{});
    }

    db.addSystem([](se::TransformComponent &trans, TempComponent&) {
        if (se::Input::checkKey(se::KeyState::PRESSED, se::Key::Z))
            trans.position.y += 5;
    });

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(-10, 10);
    db.addSystem([&dis, &gen](se::TransformComponent &trans) {
        trans.position.y += dis(gen)/100.0;
    });

    renderer.drawTextures(db);

    se::Game::Instance().init();
    se::Game::Instance().run(db, renderer);

    return 0;
}
