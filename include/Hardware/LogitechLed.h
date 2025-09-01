
#pragma once

#include "Core/Keyboard/Keyboard.h"
#include "Hardware/IHardware.h"

/**
 * @class LogitechLed
 * @brief An IHardware implementation for Logitech keyboards using the LED SDK.
 *
 * This class acts as an adapter between the abstract lighting engine and the
 * concrete Logitech hardware. It handles both rendering (output) for the G213's
 * 5-zone lighting and reading key presses (input) using the Windows API.
 * 
 * @author Michele Bisignano
 */
class LogitechLed : public IHardware {
public:
    /**
     * @brief Constructs the LogitechLed adapter.
     * @param keyboard A pointer to the keyboard model, used for mapping keys to zones.
     */
    explicit LogitechLed(const Keyboard* keyboard);

    bool initialize() override;
    void shutdown() override;
    void render(const std::vector<Color>& frameBuffer) override;
    std::vector<bool> getKeyboardState() const override;

private:
    const Keyboard* keyboard_;
};