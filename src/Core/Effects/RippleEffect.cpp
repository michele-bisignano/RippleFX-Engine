/**
 * @author Michele Bisignano
 */
#include "Core/Effects/RippleEffect.h"
#include "Core/Keyboard/KeyCodes.h"

RippleEffect::RippleEffect(const Key& startKey, const Color& color, int stepDuration, int propagationDelay, int maxLifetime)
    : color_(color),
    stepDuration_(stepDuration > 0 ? stepDuration : 1),
    propagationDelay_(propagationDelay > 0 ? propagationDelay : 1),
    maxLifetime_(maxLifetime)
{
    // We can pre-allocate memory in the hash table to improve performance.
    // 104 is the max number of keys, so this is a safe upper bound.
    activeKeys_.reserve(static_cast<size_t>(KeyCode::KEY_COUNT));
    activeKeys_[&startKey] = { State::Ignited, 0 };
}


void RippleEffect::update() {
    framesLived_++;
    if (isFinished()) {
        activeKeys_.clear();
        return;
    }

    // This map will store the complete state of the effect for the *next* frame.
    std::unordered_map<const Key*, KeyState> next_frame_states;
    next_frame_states.reserve(activeKeys_.size() + 10);

    // --- Single Pass Update Logic ---
    // Iterate through all keys that are currently active in the effect.
    for (auto const& [key, current_state] : activeKeys_) {

        // --- 1. PROPAGATE ---
        // If this key is at the crest of the wave, it should ignite its neighbors.
        if (current_state.state == State::Ignited && current_state.framesInState >= propagationDelay_) {
            for (const Key* neighbor : key->neighbors) {
                // We only ignite a neighbor if it's not already in the *next* frame's map.
                // This prevents a key from being ignited and then immediately overwritten by a
                // different state transition in the same frame.
                if (activeKeys_.find(neighbor) == activeKeys_.end()) {
                    next_frame_states[neighbor] = { State::Ignited, 0 };
                }
            }
        }

        // --- 2. TRANSITION ---
        // Now, calculate the next state for the key we are currently processing.
        KeyState next_state = current_state;
        next_state.framesInState++;

        if (next_state.framesInState >= stepDuration_) {
            next_state.framesInState = 0; // Reset counter for the new state

            if (current_state.state == State::Ignited) {
                next_state.state = State::Fading_High;
            }
            else if (current_state.state == State::Fading_High) {
                next_state.state = State::Fading_Low;
            }
            else { // The state was Fading_Low
                // The key's life is over. We remove it from the effect by simply
                // NOT adding it to the next_frame_states map.
                continue; // Skip to the next key in the for loop
            }
        }

        // --- 3. UPDATE ---
        // Place the key's calculated next state into the map for the next frame.
        // This will correctly overwrite any newly ignited neighbors if they were
        // somehow already part of the activeKeys_ map (which shouldn't happen
        // with correct propagation, but this makes the logic safer).
        next_frame_states[key] = next_state;
    }

    // Atomically update the effect's state for the next frame.
    activeKeys_ = next_frame_states;
}

Color RippleEffect::getColorForKey(const Key& key) const {
    // If the effect's lifetime is over, all keys should be black.
    if (isFinished()) {
        return Color(0, 0, 0);
    }

    auto it = activeKeys_.find(&key);
    if (it == activeKeys_.end()) {
        // This key is not currently affected by this ripple.
        return Color(0, 0, 0);
    }

    // Return a color based on the key's current state.
    switch (it->second.state) {
    case State::Ignited:
        return color_;
    case State::Fading_High:
        return color_.scale(204); // ~80%
    case State::Fading_Low:
        return color_.scale(102); // ~40%
    default:
        return Color(0, 0, 0);
    }
}

bool RippleEffect::isFinished() const {
    // The effect is now finished based on its total lifetime, not on the number of active keys.
    return framesLived_ >= maxLifetime_;
}