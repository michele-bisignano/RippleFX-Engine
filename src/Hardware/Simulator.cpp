#include "Hardware/Simulator.h"
#include <iostream>

Simulator::Simulator(const Keyboard* keyboard)
    : keyboard_(keyboard)
{
}

bool Simulator::initialize() {
    std::cout << "[Simulator] Hardware Initialized." << std::endl;
    return true;
}

void Simulator::shutdown() {
    std::cout << "[Simulator] Hardware Shutdown." << std::endl;
}

void Simulator::render(const std::vector<Color>& frameBuffer) {
    if (!keyboard_) return;

    std::cout << "--- Frame " << frameCount_ << " ---" << std::endl;
    const auto& keys = keyboard_->getKeys();

    for (size_t i = 0; i < keys.size(); ++i) {
        const Color& c = frameBuffer[i];
        // Only print keys that are not black to keep the output clean.
        if (c.getRed() > 0 || c.getGreen() > 0 || c.getBlue() > 0) {
            std::cout << "  Key ID " << keys[i].getId() << " | Color: " << c.toHex() << std::endl;
        }
    }
}
std::vector<bool> Simulator::getKeyboardState() const {
    if (!keyboard_) {
        return {};
    }

    // Create a vector to hold the state of every key, initialized to 'false'.
    std::vector<bool> keyStates(keyboard_->getKeys().size(), false);

    frameCount_++;
    // Every 150 frames, we'll simulate pressing the 'G' key.
    if (frameCount_ % 150 == 0) {
        std::cout << "\n*** SIMULATING KEY PRESS: 'G' ***\n" << std::endl;
        
        // Find the 'G' key in the layout.
        const Key* g_key = keyboard_->findKeyById(KeyCode::G);
        if (g_key) {
            // Set the state for that specific key to 'true'.
            keyStates[g_key->getIndex()] = true;
        }
    }

    return keyStates;
}