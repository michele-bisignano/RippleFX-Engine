#pragma once
#include "Core/Keyboard/Key.h"
#include "Core/Util/Color.h"
/**
 * @class IEffect
 * @brief An interface defining the contract for all lighting effects.
 *
 * This is an abstract base class. Any class that inherits from IEffect
 * must implement these pure virtual functions. This allows the LightingManager
 * to handle all effects polymorphically.
 * 
 * @author Michele Bisignano
 */
class IEffect {
public:
    /**
     * @brief Virtual destructor. Essential for any class intended for polymorphic deletion.
     */
    virtual ~IEffect() = default;

    /**
     * @brief Updates the internal state of the effect. Called once per frame.
     */
    virtual void update() = 0;

    /**
     * @brief Gets the color that this effect wants to apply to a specific key.
     * @param key The key for which to calculate the color.
     * @return The calculated Color. If the effect does not affect this key, it should return black.
     */
    virtual Color getColorForKey(const Key& key) const = 0;

    /**
     * @brief Checks if the effect has completed its lifecycle.
     * @return true if the effect is finished and can be removed, false otherwise.
     */
    virtual bool isFinished() const = 0;
};