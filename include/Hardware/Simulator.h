#pragma once

#include "Core/Keyboard/Keyboard.h" // Needed to map framebuffer indices to Key IDs
#include "Hardware/IHardware.h"

/**
 * @class Simulator
 * @brief An IHardware implementation that simulates a keyboard in the console.
 *
 * This class is used for testing and development. It "renders" the keyboard's
 * lighting state by printing color values to the console and simulates key
 * presses at a fixed interval.
 * 
 * @author Michele Bisignano
 */
class Simulator : public IHardware {
public:
    /**
     * @brief Constructs the Simulator.
     * @param keyboard A pointer to the keyboard model to map colors to key IDs for printing.
     */
    explicit Simulator(const Keyboard* keyboard);

    bool initialize() override;
    void shutdown() override;
    void render(const std::vector<Color>& frameBuffer) override;
    std::vector<bool> getKeyboardState() const override;

private:
    const Keyboard* keyboard_;
    mutable int frameCount_ = 0;
};