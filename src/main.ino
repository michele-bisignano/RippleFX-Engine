/**
 * @author Michele Bisignano
 */

// --- Core Engine Includes ---
// These are your platform-independent library files.
// You would need to add your Core/ library to the Arduino/PlatformIO project.
#include "Core/Keyboard/Keyboard.h"
#include "Core/Lighting/LightingManager.h"
#include "Core/Effects/RippleEffect.h"
#include "Hardware/IHardware.h"

// --- ESP32 Hardware Implementation (Placeholder) ---
// You would create these files to control your specific hardware (e.g., NeoPixel LEDs)
// #include "Hardware/ESP32_NeoPixel.h"
// #include "Hardware/ESP32_KeyMatrix.h"


// =========================================================================
// --- Global Objects ---
// On a microcontroller, we pre-allocate all major objects as globals
// to ensure all memory is accounted for at compile time and to avoid
// using the heap in the main loop.
// =========================================================================

Keyboard keyboard;
LightingManager lightingManager(&keyboard);

// The hardware pointer will be assigned in setup().
IHardware* hardware;

// --- Timing Configuration ---
constexpr int TARGET_FPS = 60;
constexpr unsigned long FRAME_INTERVAL_MS = 1000 / TARGET_FPS;

// --- State Variables ---
unsigned long last_update_time = 0;
std::vector<bool> previous_key_state;
unsigned long last_press_time = 0;


// =========================================================================
// --- SETUP FUNCTION ---
// This function runs once when the microcontroller boots up.
// =========================================================================
void setup() {
    // --- 1. Initialization ---

    // <<< YOU MUST IMPLEMENT THIS >>>
    // Create an instance of your concrete hardware implementation for the ESP32.
    // For example:
    // hardware = new ESP32_NeoPixel(&keyboard);

    // Initialize the hardware.
    if (!hardware->initialize()) {
        // Handle initialization failure (e.g., blink an error LED).
        while(true) {} // Halt execution
    }

    // Initialize the state vectors after the keyboard is fully constructed.
    previous_key_state.resize(keyboard.getKeys().size(), false);

    // Initialize the timers.
    last_update_time = millis();
    last_press_time = millis();

    // Optional: Start serial communication for debugging.
    // Serial.begin(115200);
    // Serial.println("System initialized. Starting main loop.");
}


// =========================================================================
// --- LOOP FUNCTION ---
// This function runs continuously in a loop as fast as possible after setup() completes.
// All logic inside must be non-blocking.
// =========================================================================
void loop() {
    // --- Non-Blocking Timer Check ---
    // Check if enough time has passed to render the next frame.
    unsigned long current_time = millis();
    if (current_time - last_update_time >= FRAME_INTERVAL_MS) {
        last_update_time = current_time; // Reset the timer for the next frame.

        // --- 3. Input Handling ---
        std::vector<bool> current_key_state = hardware->getKeyboardState();
        const auto& keys = keyboard.getKeys();

        for (size_t i = 0; i < keys.size(); ++i) {
            // Detect a new key press (rising edge).
            if (current_key_state[i] && !previous_key_state[i]) {
                const Key& pressedKey = keys[i];
                
                // --- 4. DYNAMIC EFFECT CREATION ---
                unsigned long now = millis();
                unsigned long time_since_last_press = now - last_press_time;
                last_press_time = now;

                long long lifetime_ms = (time_since_last_press << 1);
                lifetime_ms = std::max(500LL, std::min(7000LL, lifetime_ms));

                int maxLifetime = lifetime_ms >> 4; // Fast division by 16

                int propagationDelay = 5;
                if (time_since_last_press < 150) propagationDelay = 1;
                else if (time_since_last_press < 250) propagationDelay = 2;
                else if (time_since_last_press < 350) propagationDelay = 3;
                else if (time_since_last_press < 500) propagationDelay = 4;

                int stepDuration = std::max(1, maxLifetime >> 3); // Fast division by 8

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

        // --- 5. Logic Update ---
        lightingManager.update();

        // --- 6. Rendering ---
        const std::vector<Color>& frame = lightingManager.getFrameBuffer();
        hardware->render(frame);
    }
}