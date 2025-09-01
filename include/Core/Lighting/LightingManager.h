#pragma once

#pragma once
#include "Core/Keyboard/Keyboard.h"
#include "Core/Effects/IEffect.h"
#include "Core/Lighting/EffectPool.h"
#include <vector>

/**
 * @class LightingManager
 * @brief Orchestrates all active lighting effects and renders the final frame.
 *
 * This class is the core of the lighting engine. It maintains a list of
 * active effects, updates them each frame, removes finished ones, and blends
 * their outputs into a final framebuffer to be sent to the hardware.
 * 
 * @author Michele Bisignano
 */
class LightingManager {
public:
    /**
     * @brief Constructs the LightingManager.
     * @param keyboard A pointer to the keyboard model. The manager does not own this pointer.
     */
    explicit LightingManager(Keyboard* keyboard);

    /**
     * @brief Updates all active effects and renders the next frame. This should be called once per frame.
     */
    void update();

    /**
     * @brief Creates a new ripple effect and adds it to the list of active effects.
     *
     * This method serves as a factory for creating RippleEffect objects. It uses the
     * internal EffectPool to acquire memory for the new effect, avoiding real-time
     * heap allocation. If the effect is created successfully, it is added to the
     * manager's list of active effects, which will be updated and rendered in
     * subsequent frames.
     *
     * @note If the EffectPool is full, the creation will fail, and this function
     *       will do nothing (the request is silently ignored).
     *
     * @param startKey The key where the ripple effect originates.
     * @param color The color of the ripple.
     * @param stepDuration The time in milliseconds for each step of the ripple's expansion.
     * @param propagationDelay The delay in milliseconds between each propagation step.
     * @param maxLifetime The total duration in milliseconds the effect should last before being removed.
     * @see EffectPool::create()
     */
    void addRippleEffect(const Key& startKey, const Color& color, int stepDuration, int propagationDelay, int maxLifetime);
    
    /**
     * @brief Gets the final, blended colors for the current frame.
     * @return A const reference to the framebuffer vector. The size of this vector
     *         matches the number of keys on the keyboard.
     */
    const std::vector<Color>& getFrameBuffer() const;

private:
    Keyboard* keyboard_;
    EffectPool effectPool_;
    std::vector<IEffect*> activeEffects_;
    std::vector<Color> frameBuffer_; // One color for each key, indexed implicitly
};