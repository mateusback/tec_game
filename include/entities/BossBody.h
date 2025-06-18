#ifndef BOSSBODY_H
#define BOSSBODY_H
#include <memory>

#include "MovingBody.h"
#include "PlayerBody.h"

namespace Entities {
    class BossBody : public MovingBody  {
    private:
        Enemies::BossData bossData;
        std::unique_ptr<PlayerBody> target = nullptr;
        float agrroRange = 0.f;
    public:
    };
}

#endif
