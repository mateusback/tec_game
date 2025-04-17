#ifndef ENEMY_H
#define ENEMY_H

#include <string>

namespace Enemies {

    class Enemy {
    private:
        int id;
        std::string name;
        std::string spritePath;
        float acceleration;
        float aggroRange;
        std::string behavior;

    public:
        Enemy(int id = -1, std::string name = "", std::string sprite = "", float acceleration = 100.f, float range = 100.f, std::string behavior = "")
            : id(id), name(name), spritePath(sprite), acceleration(acceleration), aggroRange(range), behavior(behavior) {}

        int getId() const { return id; }
        const std::string& getName() const { return name; }
        const std::string& getSpritePath() const { return spritePath; }
        float getAcceleration() const { return acceleration; }
        float getAggroRange() const { return aggroRange; }
        const std::string& getBehavior() const { return behavior; }
    };
}

#endif
