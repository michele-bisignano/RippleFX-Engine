#pragma once

#include "Core/Keyboard/KeyCodes.h"
#include "Core/Util/Color.h"
#include <vector>

/**
 * @class IHardware
 * @brief An interface defining the contract for all hardware backends.
 *
 * This is an abstract base class that defines the essential operations
 * for any hardware that the lighting engine can interact with. This allows
 * for easy swapping between a simulator and real hardware like a Logitech keyboard.
 * 
 * @author Michele Bisignano
 */
class IHardware {
public:
    /**
     * @brief Virtual destructor. Essential for any class intended for polymorphic deletion.
     */
    virtual ~IHardware() = default;

    /**
     * @brief Initializes the connection to the hardware or simulator.
     * @return true if initialization was successful, false otherwise.
     */
    virtual bool initialize() = 0;

    /**
     * @brief Shuts down the connection to the hardware, releasing any resources.
     */
    virtual void shutdown() = 0;

    /**
     * @brief Sends the final, calculated color data to the hardware.
     * @param frameBuffer A vector of Colors representing the state of every key.
     */
    virtual void render(const std::vector<Color>& frameBuffer) = 0;

    /**
     * @brief Gets the current state of every key on the keyboard.
     * @return A vector of booleans, where the index corresponds to a key's index
     *         in the Keyboard layout. 'true' means the key is currently held down.
     */
    virtual std::vector<bool> getKeyboardState() const = 0;
};