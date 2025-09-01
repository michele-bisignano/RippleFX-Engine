#include "Core/Lighting/LightingManager.h"
#include <algorithm>

LightingManager::LightingManager(Keyboard* keyboard)
    : keyboard_(keyboard)
{
    // Initialize the framebuffer to the correct size, filled with black
    if (keyboard_) {
        frameBuffer_.resize(keyboard_->getKeys().size(), Color(0, 0, 0));
    }
}

void LightingManager::update() {
    if (!keyboard_) return;

    // --- 1. Update all active effects ---
    for (auto& effect : activeEffects_) {
        effect->update();
    }

    // --- 2. Remove any effects that have finished ---
    // The removal logic changes because we are using raw pointers and a pool.
    // An explicit iterator loop is the clearest way to handle this.
    auto it = activeEffects_.begin();
    while (it != activeEffects_.end()) {
        if ((*it)->isFinished()) {
            // Return the effect's memory to the pool.
            // A cast is necessary because destroy() expects the concrete type.
            effectPool_.destroy(static_cast<RippleEffect*>(*it));

            // Remove the pointer from the vector.
            // erase() returns an iterator to the next valid element.
            it = activeEffects_.erase(it);
        }
        else {
            // If the effect is not finished, just advance to the next one.
            ++it;
        }
    }

    // --- 3. Render the final frame ---
    // Start with a black frame
    frameBuffer_.assign(keyboard_->getKeys().size(), Color(0, 0, 0));

    const auto& keys = keyboard_->getKeys();
    for (size_t i = 0; i < keys.size(); ++i) {
        // For each key, additively blend the colors from all active effects.
        for (const auto& effect : activeEffects_) {
            Color effectColor = effect->getColorForKey(keys[i]);
            // Use the new 'add' method to layer the lights.
            frameBuffer_[i] = frameBuffer_[i].add(effectColor);
        }
    }
}

void LightingManager::addRippleEffect(const Key& startKey, const Color& color, int stepDuration, int propagationDelay, int maxLifetime) {
    RippleEffect* new_effect = effectPool_.create(startKey, color, stepDuration, propagationDelay, maxLifetime);
    if (new_effect) {
        activeEffects_.push_back(static_cast<IEffect*>(new_effect));
    }
}

const std::vector<Color>& LightingManager::getFrameBuffer() const {
    return frameBuffer_;
}