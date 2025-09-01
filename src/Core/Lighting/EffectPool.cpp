/**
 * @author Michele Bisignano
 */
#include "Core/Lighting/EffectPool.h"

EffectPool::EffectPool() {
    // At the start, all memory slots are free.
    // We fill our freeSlots_ vector with pointers to the start of each "room".
    freeSlots_.reserve(MAX_ACTIVE_EFFECTS);
    for (size_t i = 0; i < MAX_ACTIVE_EFFECTS; ++i) {
        RippleEffect* slot = reinterpret_cast<RippleEffect*>(&memoryPool_[i * sizeof(RippleEffect)]);
        freeSlots_.push_back(slot);
    }
}

void EffectPool::destroy(RippleEffect* effect) {
    if (effect) {
        // Explicitly call the destructor of the object.
        effect->~RippleEffect();
        // Add the memory slot back to the list of available "rooms".
        freeSlots_.push_back(effect);
    }
}