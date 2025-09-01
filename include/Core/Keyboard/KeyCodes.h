// include/Keyboard/KeyCodes.h

#pragma once
#include <cstdint>

// This file defines unique identifiers for each key.
// In a real product, these would likely correspond to USB HID Usage IDs.

// Using an enum class for type safety
enum class KeyCode {
    // This enum represents all the keys we want to map.
    // It's based on a standard US QWERTY layout for mapping purposes,
    // but the names are universal.

    // Alphanumeric Keys
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,

    // Function Keys
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

    // Modifier Keys
    LEFT_SHIFT, RIGHT_SHIFT,
    LEFT_CONTROL, RIGHT_CONTROL,
    LEFT_ALT, RIGHT_ALT,
    LEFT_WINDOWS, RIGHT_WINDOWS,
    CAPS_LOCK,

    // Special Keys
    ESCAPE,
    SPACE,
    ENTER,
    BACKSPACE,
    TAB,
    CONTEXT_MENU, // The key that opens the right-click menu

    // Navigation and Editing
    INSERT, DELETE_KEY, // Renamed to avoid conflict with DELETE macro
    HOME, END,
    PAGE_UP, PAGE_DOWN,
    ARROW_UP, ARROW_DOWN, ARROW_LEFT, ARROW_RIGHT,

    // System Keys
    PRINT_SCREEN, SCROLL_LOCK, PAUSE_BREAK,

    // Numpad Keys
    NUMPAD_0, NUMPAD_1, NUMPAD_2, NUMPAD_3, NUMPAD_4,
    NUMPAD_5, NUMPAD_6, NUMPAD_7, NUMPAD_8, NUMPAD_9,
    NUM_LOCK,
    NUMPAD_DIVIDE, NUMPAD_MULTIPLY, NUMPAD_SUBTRACT, NUMPAD_ADD,
    NUMPAD_ENTER, NUMPAD_DECIMAL,

    // OEM / Punctuation Keys (based on US layout positions)
    // These may correspond to different characters on your Italian layout.
    OEM_TILDE,      // `~` (Often `\` on non-US keyboards)
    OEM_MINUS,      // -_
    OEM_PLUS,       // =+
    OEM_LBRACKET,   // [{
    OEM_RBRACKET,   // ]}
    OEM_BACKSLASH,  // \| (Often `<>` on non-US keyboards)
    OEM_SEMICOLON,  // ;:
    OEM_QUOTE,      // '"
    OEM_COMMA,      // ,<
    OEM_PERIOD,     // .>
    OEM_SLASH,      // /?
    OEM_102,        // Special key on non-US 102-key keyboards, often `< >` or `\|`

    KEY_COUNT
};