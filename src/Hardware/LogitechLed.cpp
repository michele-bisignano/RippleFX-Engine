#include "Hardware/LogitechLed.h"
#include "LogitechLEDLib.h"
#include <Windows.h>
#include <iostream> // For logging errors
#include <map>
#include <vector>

// --- G213 Zone Mapping ---
// This map translates a KeyCode from our engine to a specific lighting zone (1-5) on the G213.
// NOTE: This is an approximation and may need to be adjusted by experimentation.
/**
 * @brief Maps each KeyCode to one of the 5 lighting zones of the Logitech G213 keyboard.
 *
 * This map is specific to the G213's hardware layout. It's used to determine
 * which of the five zones should be lit up when a specific key is part of an effect.
 * This allows simulating per-key lighting on a zone-based keyboard.
 */
static const std::map<KeyCode, int> g213_key_to_zone_map = {
    // --- Zone 1 ---
    {KeyCode::ESCAPE, 1}, {KeyCode::F1, 1}, {KeyCode::F2, 1}, {KeyCode::F3, 1}, {KeyCode::F4, 1}, {KeyCode::F5, 1},
    {KeyCode::OEM_TILDE, 1}, {KeyCode::NUM_1, 1}, {KeyCode::NUM_2, 1}, {KeyCode::NUM_3, 1}, {KeyCode::NUM_4, 1}, {KeyCode::NUM_5, 1},
    {KeyCode::TAB, 1}, {KeyCode::Q, 1}, {KeyCode::W, 1}, {KeyCode::E, 1}, {KeyCode::R, 1}, {KeyCode::T, 1},
    {KeyCode::CAPS_LOCK, 1}, {KeyCode::A, 1}, {KeyCode::S, 1}, {KeyCode::D, 1}, {KeyCode::F, 1}, {KeyCode::G, 1},
    {KeyCode::LEFT_SHIFT, 1}, {KeyCode::Z, 1}, {KeyCode::X, 1}, {KeyCode::C, 1}, {KeyCode::V, 1},
    {KeyCode::LEFT_CONTROL, 1}, {KeyCode::LEFT_WINDOWS, 1}, {KeyCode::LEFT_ALT, 1},

    // --- Zone 2 ---
    {KeyCode::F6, 2}, {KeyCode::F7, 2}, {KeyCode::F8, 2},
    {KeyCode::NUM_6, 2}, {KeyCode::NUM_7, 2}, {KeyCode::NUM_8, 2},
    {KeyCode::Y, 2}, {KeyCode::U, 2}, {KeyCode::I, 2},
    {KeyCode::H, 2}, {KeyCode::J, 2}, {KeyCode::K, 2},
    {KeyCode::B, 2}, {KeyCode::N, 2}, {KeyCode::M, 2},

    // --- Zone 3 ---
    {KeyCode::F9, 3}, {KeyCode::F10, 3}, {KeyCode::F11, 3}, {KeyCode::F12, 3},
    {KeyCode::NUM_9, 3}, {KeyCode::NUM_0, 3}, {KeyCode::OEM_MINUS, 3}, {KeyCode::OEM_PLUS, 3}, {KeyCode::BACKSPACE, 3},
    {KeyCode::O, 3}, {KeyCode::P, 3}, {KeyCode::OEM_LBRACKET, 3}, {KeyCode::OEM_RBRACKET, 3}, {KeyCode::OEM_BACKSLASH, 3},
    {KeyCode::L, 3}, {KeyCode::OEM_SEMICOLON, 3}, {KeyCode::OEM_QUOTE, 3}, {KeyCode::ENTER, 3},
    {KeyCode::OEM_COMMA, 3}, {KeyCode::OEM_PERIOD, 3}, {KeyCode::OEM_SLASH, 3}, {KeyCode::RIGHT_SHIFT, 3},
    {KeyCode::SPACE, 3}, {KeyCode::RIGHT_ALT, 3}, {KeyCode::CONTEXT_MENU, 3}, {KeyCode::RIGHT_CONTROL, 3},

    // --- Zone 4 ---
    {KeyCode::PRINT_SCREEN, 4}, {KeyCode::SCROLL_LOCK, 4}, {KeyCode::PAUSE_BREAK, 4},
    {KeyCode::INSERT, 4}, {KeyCode::HOME, 4}, {KeyCode::PAGE_UP, 4},
    {KeyCode::DELETE_KEY, 4}, {KeyCode::END, 4}, {KeyCode::PAGE_DOWN, 4},
    {KeyCode::ARROW_UP, 4}, {KeyCode::ARROW_LEFT, 4}, {KeyCode::ARROW_DOWN, 4}, {KeyCode::ARROW_RIGHT, 4},

    // --- Zone 5 (Numpad) ---
    {KeyCode::NUM_LOCK, 5}, {KeyCode::NUMPAD_DIVIDE, 5}, {KeyCode::NUMPAD_MULTIPLY, 5}, {KeyCode::NUMPAD_SUBTRACT, 5},
    {KeyCode::NUMPAD_7, 5}, {KeyCode::NUMPAD_8, 5}, {KeyCode::NUMPAD_9, 5}, {KeyCode::NUMPAD_ADD, 5},
    {KeyCode::NUMPAD_4, 5}, {KeyCode::NUMPAD_5, 5}, {KeyCode::NUMPAD_6, 5},
    {KeyCode::NUMPAD_1, 5}, {KeyCode::NUMPAD_2, 5}, {KeyCode::NUMPAD_3, 5}, {KeyCode::NUMPAD_ENTER, 5},
    {KeyCode::NUMPAD_0, 5}, {KeyCode::NUMPAD_DECIMAL, 5}
};
static const std::map<int, KeyCode> vk_to_keycode_map = {
    // --- Alphanumeric Keys ---
    { 'A', KeyCode::A }, { 'B', KeyCode::B }, { 'C', KeyCode::C }, { 'D', KeyCode::D },
    { 'E', KeyCode::E }, { 'F', KeyCode::F }, { 'G', KeyCode::G }, { 'H', KeyCode::H },
    { 'I', KeyCode::I }, { 'J', KeyCode::J }, { 'K', KeyCode::K }, { 'L', KeyCode::L },
    { 'M', KeyCode::M }, { 'N', KeyCode::N }, { 'O', KeyCode::O }, { 'P', KeyCode::P },
    { 'Q', KeyCode::Q }, { 'R', KeyCode::R }, { 'S', KeyCode::S }, { 'T', KeyCode::T },
    { 'U', KeyCode::U }, { 'V', KeyCode::V }, { 'W', KeyCode::W }, { 'X', KeyCode::X },
    { 'Y', KeyCode::Y }, { 'Z', KeyCode::Z },
    { '0', KeyCode::NUM_0 }, { '1', KeyCode::NUM_1 }, { '2', KeyCode::NUM_2 },
    { '3', KeyCode::NUM_3 }, { '4', KeyCode::NUM_4 }, { '5', KeyCode::NUM_5 },
    { '6', KeyCode::NUM_6 }, { '7', KeyCode::NUM_7 }, { '8', KeyCode::NUM_8 },
    { '9', KeyCode::NUM_9 },

    // --- Function Keys ---
    { VK_F1, KeyCode::F1 }, { VK_F2, KeyCode::F2 }, { VK_F3, KeyCode::F3 },
    { VK_F4, KeyCode::F4 }, { VK_F5, KeyCode::F5 }, { VK_F6, KeyCode::F6 },
    { VK_F7, KeyCode::F7 }, { VK_F8, KeyCode::F8 }, { VK_F9, KeyCode::F9 },
    { VK_F10, KeyCode::F10 }, { VK_F11, KeyCode::F11 }, { VK_F12, KeyCode::F12 },

    // --- Modifier Keys ---
    { VK_LSHIFT, KeyCode::LEFT_SHIFT }, { VK_RSHIFT, KeyCode::RIGHT_SHIFT },
    { VK_LCONTROL, KeyCode::LEFT_CONTROL }, { VK_RCONTROL, KeyCode::RIGHT_CONTROL },
    { VK_LMENU, KeyCode::LEFT_ALT }, { VK_RMENU, KeyCode::RIGHT_ALT },
    { VK_LWIN, KeyCode::LEFT_WINDOWS }, { VK_RWIN, KeyCode::RIGHT_WINDOWS },
    { VK_CAPITAL, KeyCode::CAPS_LOCK },

    // --- Special Keys ---
    { VK_ESCAPE, KeyCode::ESCAPE },
    { VK_SPACE, KeyCode::SPACE },
    { VK_RETURN, KeyCode::ENTER },
    { VK_BACK, KeyCode::BACKSPACE },
    { VK_TAB, KeyCode::TAB },
    { VK_APPS, KeyCode::CONTEXT_MENU },

    // --- Navigation and Editing ---
    { VK_INSERT, KeyCode::INSERT }, { VK_DELETE, KeyCode::DELETE_KEY },
    { VK_HOME, KeyCode::HOME }, { VK_END, KeyCode::END },
    { VK_PRIOR, KeyCode::PAGE_UP }, { VK_NEXT, KeyCode::PAGE_DOWN },
    { VK_UP, KeyCode::ARROW_UP }, { VK_DOWN, KeyCode::ARROW_DOWN },
    { VK_LEFT, KeyCode::ARROW_LEFT }, { VK_RIGHT, KeyCode::ARROW_RIGHT },

    // --- System Keys ---
    { VK_PRINT, KeyCode::PRINT_SCREEN }, { VK_SCROLL, KeyCode::SCROLL_LOCK },
    { VK_PAUSE, KeyCode::PAUSE_BREAK },

    // --- Numpad Keys ---
    { VK_NUMPAD0, KeyCode::NUMPAD_0 }, { VK_NUMPAD1, KeyCode::NUMPAD_1 },
    { VK_NUMPAD2, KeyCode::NUMPAD_2 }, { VK_NUMPAD3, KeyCode::NUMPAD_3 },
    { VK_NUMPAD4, KeyCode::NUMPAD_4 }, { VK_NUMPAD5, KeyCode::NUMPAD_5 },
    { VK_NUMPAD6, KeyCode::NUMPAD_6 }, { VK_NUMPAD7, KeyCode::NUMPAD_7 },
    { VK_NUMPAD8, KeyCode::NUMPAD_8 }, { VK_NUMPAD9, KeyCode::NUMPAD_9 },
    { VK_NUMLOCK, KeyCode::NUM_LOCK },
    { VK_DIVIDE, KeyCode::NUMPAD_DIVIDE }, { VK_MULTIPLY, KeyCode::NUMPAD_MULTIPLY },
    { VK_SUBTRACT, KeyCode::NUMPAD_SUBTRACT }, { VK_ADD, KeyCode::NUMPAD_ADD },
    { VK_SEPARATOR, KeyCode::NUMPAD_ENTER }, // Numpad Enter often has a different code
    { VK_DECIMAL, KeyCode::NUMPAD_DECIMAL },

    // --- OEM / Punctuation Keys (based on US layout positions) ---
    // NOTE: These VK codes correspond to key POSITIONS, not the characters printed on them.
    // The characters will change with the keyboard layout (e.g., Italian).
    { VK_OEM_3, KeyCode::OEM_TILDE },      // In IT layout, this is 'ù'
    { VK_OEM_MINUS, KeyCode::OEM_MINUS },    // In IT layout, this is '-'
    { VK_OEM_PLUS, KeyCode::OEM_PLUS },       // In IT layout, this is '+'
    { VK_OEM_4, KeyCode::OEM_LBRACKET },   // In IT layout, this is 'è'
    { VK_OEM_6, KeyCode::OEM_RBRACKET },   // In IT layout, this is 'ì'
    { VK_OEM_5, KeyCode::OEM_BACKSLASH },  // In IT layout, this is '\'
    { VK_OEM_1, KeyCode::OEM_SEMICOLON },  // In IT layout, this is 'ò'
    { VK_OEM_7, KeyCode::OEM_QUOTE },      // In IT layout, this is '\''
    { VK_OEM_COMMA, KeyCode::OEM_COMMA },    // In IT layout, this is ','
    { VK_OEM_PERIOD, KeyCode::OEM_PERIOD },   // In IT layout, this is '.'
    { VK_OEM_2, KeyCode::OEM_SLASH },      // In IT layout, this is 'à'
    { VK_OEM_102, KeyCode::OEM_102 }       // In IT layout, this is '< >'
};

LogitechLed::LogitechLed(const Keyboard* keyboard)
    : keyboard_(keyboard)
{
}

bool LogitechLed::initialize() {
    if (LogiLedInitWithName("RippleEffectEngine")) {
        std::cout << "[Logitech] SDK Initialized successfully." << std::endl;
        return true;
    }
    std::cerr << "[Logitech] ERROR: Failed to initialize SDK. Is G HUB running?" << std::endl;
    return false;
}

void LogitechLed::shutdown() {
    LogiLedShutdown();
    std::cout << "[Logitech] SDK Shutdown." << std::endl;
}

// Helper function to perform the fast, multiplication-free conversion.
// This can be placed at the top of the .cpp file.
inline int convert_255_to_100(int value) {
    // Approximates (value * 100) / 256 using only bit shifts and additions.
    // This is extremely fast on a microprocessor.
    // (value * 64) + (value * 32) + (value * 4)
    int multiplied = (value << 6) + (value << 5) + (value << 2);
    return multiplied >> 8;
}

void LogitechLed::render(const std::vector<Color>& frameBuffer) {
    if (!keyboard_) return;

    
    // --- "Brightest Key" Logic for 5 Zones ---
    const int NUM_ZONES = 5;

    // Step A: Create a data structure to hold the brightest color found for each zone so far.
    // Initialize all zones to black.
    std::vector<Color> brightest_zone_colors(NUM_ZONES, Color(0, 0, 0));

    // Step B: Iterate through the ideal per-key framebuffer.
    const auto& all_keys = keyboard_->getKeys();
    for (size_t i = 0; i < all_keys.size(); ++i) {
        auto it = g213_key_to_zone_map.find(static_cast<KeyCode>(all_keys[i].getId()));
        if (it != g213_key_to_zone_map.end()) {
            int zone_index = it->second - 1; // Map zones 1-5 to indices 0-4
            const Color& current_key_color = frameBuffer[i];

            // Compare the brightness of the current key's color with the
            // brightest color we've found for this zone so far.
            // A simple brightness metric is to sum the R, G, and B components.
            int current_brightness = current_key_color.getRed() + current_key_color.getGreen() + current_key_color.getBlue();
            int max_brightness_found = brightest_zone_colors[zone_index].getRed() + brightest_zone_colors[zone_index].getGreen() + brightest_zone_colors[zone_index].getBlue();

            // If the current key is brighter, it becomes the new color for the entire zone.
            if (current_brightness > max_brightness_found) {
                brightest_zone_colors[zone_index] = current_key_color;
            }
        }
    }

    // Step C: Send the final 5 "brightest" colors to the Logitech SDK.
    for (int i = 0; i < NUM_ZONES; ++i) {
        Color final_zone_color = brightest_zone_colors[i];

        // The SDK expects colors as percentages (0-100).
        LogiLedSetLightingForTargetZone(
            LogiLed::DeviceType::Keyboard,
            i + 1, // SDK zones are 1-indexed
            convert_255_to_100(final_zone_color.getRed()),
            convert_255_to_100(final_zone_color.getGreen()),
            convert_255_to_100(final_zone_color.getBlue())
        );
    }
}
std::vector<bool> LogitechLed::getKeyboardState() const {
    // Crea un vettore per contenere lo stato di ogni tasto, inizializzato a 'false'.
    std::vector<bool> keyStates(keyboard_->getKeys().size(), false);

    // Itera attraverso la nostra mappa di traduzione.
    for (const auto& pair : vk_to_keycode_map) {
        int vk_code = pair.first;
        KeyCode key_code = pair.second;

        // GetAsyncKeyState restituisce un valore il cui bit più significativo è 1 se il tasto è premuto.
        // Usiamo una maschera bit a bit per controllarlo.
        if (GetAsyncKeyState(vk_code) & 0x8000) {
            // Il tasto è premuto. Troviamo il suo oggetto Key nel nostro layout.
            const Key* key = keyboard_->findKeyById(key_code);
            if (key) {
                // Impostiamo il valore corrispondente nel nostro vettore di stato a 'true'.
                keyStates[key->getIndex()] = true;
            }
        }
    }
    return keyStates;
}