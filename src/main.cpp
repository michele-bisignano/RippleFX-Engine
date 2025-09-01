// src/main.cpp
/**
 * @author Michele Bisignano
 */

#include "Core/Keyboard/Keyboard.h"
#include "Core/Lighting/LightingManager.h"
#include "Core/Effects/RippleEffect.h"
#include "Hardware/IHardware.h"
#include "Hardware/LogitechLed.h"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <algorithm>

 // --- High-Precision Timing Configuration ---
constexpr int TARGET_FPS = 60;
constexpr auto FRAME_DURATION = std::chrono::nanoseconds(1000000000 / TARGET_FPS);
// We use a compile-time constant for the frame duration in milliseconds for fast integer math.
// 1000 / 60 = 16. This is what we will divide by using a bit shift.
constexpr int FRAME_DURATION_MS = 16;

/**
 * @brief The main entry point of the application.
 */
int main() {
    std::cout << "RippleEffectEngine starting up..." << std::endl;

    // --- 1. Initialization ---
    Keyboard keyboard;
    std::unique_ptr<IHardware> hardware = std::make_unique<LogitechLed>(&keyboard);

    if (!hardware->initialize()) {
        std::cerr << "ERROR: Could not initialize hardware. Check that G HUB is running. Exiting." << std::endl;
        std::cin.get();
        return 1;
    }

    LightingManager lightingManager(&keyboard);
    std::cout << "System initialized. Starting main loop." << std::endl;

    auto last_update_time = std::chrono::high_resolution_clock::now();
    std::vector<bool> previous_key_state(keyboard.getKeys().size(), false);
    auto last_press_time = std::chrono::high_resolution_clock::now();

    // --- 2. Main Application Loop (Non-Blocking) ---
    while (true) {
        auto current_time = std::chrono::high_resolution_clock::now();
        if (current_time - last_update_time >= FRAME_DURATION) {
            last_update_time = current_time;

            // --- 3. Input Handling ---
            std::vector<bool> current_key_state = hardware->getKeyboardState();
            const auto& keys = keyboard.getKeys();

            for (size_t i = 0; i < keys.size(); ++i) {
                if (current_key_state[i] && !previous_key_state[i]) {
                    const Key& pressedKey = keys[i];

                    // --- 4. DYNAMIC EFFECT CREATION ---
                    auto now = std::chrono::high_resolution_clock::now();
                    auto time_since_last_press = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_press_time);
                    last_press_time = now;

                    // Use 'long long' for the intermediate calculation. This is the safest approach
                    // to prevent an integer overflow bug if the time since the last press is very long.
                    long long lifetime_ms = (time_since_last_press.count() << 1);

                    // Clamp the value to a sensible range. The 'LL' suffix ensures the numbers
                    // are treated as long long, preventing compiler warnings.
                    lifetime_ms = std::max(500LL, std::min(7000LL, lifetime_ms));

                    // Convert lifetime in milliseconds to frames using a fast bit shift (division by 16).
                    int maxLifetime = lifetime_ms >> 4;

                    // Map typing speed to wave propagation speed.
                    int propagationDelay = 5;
                    if (time_since_last_press.count() < 150) propagationDelay = 1;
                    else if (time_since_last_press.count() < 250) propagationDelay = 2;
                    else if (time_since_last_press.count() < 350) propagationDelay = 3;
                    else if (time_since_last_press.count() < 500) propagationDelay = 4;

                    // Calculate fade duration using a fast bit shift (division by 8).
                    int stepDuration = std::max(1, maxLifetime >> 3);

                    std::cout << "\n*** KEY PRESS DETECTED (ID " << pressedKey.getId() << ") ***" << std::endl;
                    std::cout << "  > Time since last press: " << time_since_last_press.count() << "ms" << std::endl;
                    std::cout << "  > New Lifetime: " << maxLifetime << " frames" << std::endl;
                    std::cout << "  > New Propagation Delay: " << propagationDelay << std::endl;
                    std::cout << "  > New Fade Step Duration: " << stepDuration << std::endl;

                    lightingManager.addRippleEffect(
                        pressedKey,
                        Color::randomColor(),
                        stepDuration,
                        propagationDelay,
                        maxLifetime
                    );
                }
            }
            previous_key_state = current_key_state;

            // --- 5. Logic Update & 6. Rendering ---
            lightingManager.update();
            hardware->render(lightingManager.getFrameBuffer());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(0));
    }

    hardware->shutdown();
    return 0;
}