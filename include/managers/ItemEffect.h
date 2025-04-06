#ifndef ITEM_EFFECT_H
#define ITEM_EFFECT_H

enum class EEffectTarget {
    AttackDamage,
    AttackSpeed,
    AttackRange,
    AttackDuration,
    FireRate,
    Defense,
    Health,
    MaxHealth
};

struct ItemEffect {
    EEffectTarget target;
    float value;
};

#endif
