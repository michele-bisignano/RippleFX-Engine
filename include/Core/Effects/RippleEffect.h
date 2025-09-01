#pragma once
#include "Core/Effects/IEffect.h"
#include <unordered_map> // Use the faster hash table instead of std::map

/**
 * @class RippleEffect
 * @brief A cellular automata-based ripple with controllable step duration.
 *
 * This effect creates a propagating wave of light. The duration of each
 * brightness step (Ignited, Fading_High, Fading_Low) is configurable,
 * allowing for effects that can be fast with a long, slow fade, or vice-versa.
 * 
 * @author Michele Bisignano
 */
class RippleEffect : public IEffect {
private:
    /**
     * @enum State
     * @brief Defines the discrete brightness levels for a key in the effect.
     */
    enum class State {Ignited, Fading_High, Fading_Low };


    /**
     * @struct KeyState
     * @brief Holds the state for a single key within this effect's animation.
     */
    struct KeyState {
        State state;
        int framesInState = 0; // Counter for how many frames the key has been in its current state.
    };

public:
    /**
     * @brief Constructs a new RippleEffect.
     * @param startKey The key where the ripple originates.
     * @param color The color of the ripple.
     * @param stepDuration The number of frames each key will spend in each brightness state (your 'X').
     * @param propagationDelay The number of frames to wait before the wave expands to the next ring of keys.
     */
    RippleEffect(const Key& startKey, const Color& color, int stepDuration, int propagationDelay, int maxLifetime);
    
    /**
     * @brief Updates the state of the ripple for the next frame.
     */
    void update() override;

    /**
     * @brief Gets the color for a specific key based on the ripple's current state.
     */
    Color getColorForKey(const Key& key) const override;

    /**
     * @brief Checks if the effect has completed.
     */
    bool isFinished() const override;

private:
    // Use std::unordered_map for fast, O(1) average lookup time while maintaining decoupling.
    std::unordered_map<const Key*, KeyState> activeKeys_;

    const Color color_;
    const int stepDuration_;
    const int propagationDelay_;
    int framesLived_ = 0;
    const int maxLifetime_;
};