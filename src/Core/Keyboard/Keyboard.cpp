// src/Keyboard/Keyboard.cpp

#include "Core/Keyboard/Keyboard.h"
#include "Core/Keyboard/KeyCodes.h"
#include <algorithm>

Keyboard::Keyboard() {
	keys_.reserve(static_cast<size_t>(KeyCode::KEY_COUNT));
	initializeLayout();
	// Assign indices to all keys
	for (size_t i = 0; i < keys_.size(); ++i) {
		keys_[i].index_ = i;
	}

	buildNeighborMaps(); // Pre-calculate neighbors after creating keys
}

const std::vector<Key>& Keyboard::getKeys() const {
	return keys_;
}

Key* Keyboard::findKeyById(KeyCode id) {
	auto it = std::find_if(keys_.begin(), keys_.end(), [id](const Key& key) {
		// Apply the same fix here for consistency.
		return key.getId() == static_cast<uint16_t>(id);
		});
	// The const_cast is still needed here to return a non-const pointer.
	return (it != keys_.end()) ? const_cast<Key*>(&(*it)) : nullptr;
}

void Keyboard::buildNeighborMaps() {
	// --- TUNING CONSTANT ---
	// This constant is local to this function because it's an implementation detail
	// of how we build the neighbor map. It does not need to be in the header file.
	constexpr float NEIGHBOR_DISTANCE_THRESHOLD = 1.6f;

	// Iterate using indices to get non-const access to both keys,
	// which is safer and avoids complex casting.
	for (size_t i = 0; i < keys_.size(); ++i) {
		for (size_t j = 0; j < keys_.size(); ++j) {
			if (i == j) continue; // A key is not a neighbor of itself

			Key& key_a = keys_[i];
			Key& key_b = keys_[j];

			// Using Manhattan distance is a fast and effective heuristic for grid-like layouts.
			// A distance of ~2 means they are immediate neighbors.
			// We use a small tolerance (e.g., 2f) to account for slight layout imperfections.
			if (key_a.getPosition().distanceManhattan(key_b.getPosition()) < NEIGHBOR_DISTANCE_THRESHOLD) {
				// Both key_a and key_b are non-const, so we can safely take the address
				// of key_b to get a Key*, which can then be implicitly and safely
				// converted to the const Key* that the vector expects.
				key_a.neighbors.push_back(&key_b);
			}
		}
	}
}

void Keyboard::initializeLayout() {
    keys_.reserve(static_cast<uint16_t>(KeyCode::KEY_COUNT)); // Pre-allocate memory for performance

    // --- Row 0: Function Row (Y = 0.0) ---
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::ESCAPE), Position(0.0f, 0.0f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::F1), Position(2.0f, 0.0f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::F2), Position(3.0f, 0.0f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::F3), Position(4.0f, 0.0f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::F4), Position(5.0f, 0.0f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::F5), Position(6.25f, 0.0f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::F6), Position(7.25f, 0.0f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::F7), Position(8.25f, 0.0f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::F8), Position(9.25f, 0.0f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::F9), Position(10.5f, 0.0f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::F10), Position(11.5f, 0.0f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::F11), Position(12.5f, 0.0f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::F12), Position(13.5f, 0.0f));

    // --- Row 1: Number Row (Y = 1.25) ---
    float x = 0.0f;
    float y = 1.25f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::OEM_TILDE), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUM_1), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUM_2), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUM_3), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUM_4), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUM_5), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUM_6), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUM_7), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUM_8), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUM_9), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUM_0), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::OEM_MINUS), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::OEM_PLUS), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::BACKSPACE), Position(x + 0.5f, y)); // 2.0 units wide

    // --- Row 2: QWERTY Row (Y = 2.25) ---
    x = 0.0f;
    y = 2.25f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::TAB), Position(x + 0.25f, y)); x += 1.5f; // 1.5 units wide
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::Q), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::W), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::E), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::R), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::T), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::Y), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::U), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::I), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::O), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::P), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::OEM_LBRACKET), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::OEM_RBRACKET), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::OEM_BACKSLASH), Position(x + 0.25f, y)); // 1.5 units wide

    // --- Row 3: Home Row (Y = 3.25) ---
    x = 0.0f;
    y = 3.25f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::CAPS_LOCK), Position(x + 0.375f, y)); x += 1.75f; // 1.75 units wide
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::A), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::S), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::D), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::F), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::G), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::H), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::J), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::K), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::L), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::OEM_SEMICOLON), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::OEM_QUOTE), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::ENTER), Position(x + 0.625f, y)); // 2.25 units wide

    // --- Row 4: Bottom Row (Y = 4.25) ---
    x = 0.0f;
    y = 4.25f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::LEFT_SHIFT), Position(x + 0.625f, y)); x += 2.25f; // 2.25 units wide
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::Z), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::X), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::C), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::V), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::B), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::N), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::M), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::OEM_COMMA), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::OEM_PERIOD), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::OEM_SLASH), Position(x, y)); x += 1.0f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::RIGHT_SHIFT), Position(x + 0.875f, y)); // 2.75 units wide

    // --- Row 5: Modifier Row (Y = 5.25) ---
    x = 0.0f;
    y = 5.25f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::LEFT_CONTROL), Position(x + 0.25f, y)); x += 1.5f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::LEFT_WINDOWS), Position(x, y)); x += 1.25f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::LEFT_ALT), Position(x, y)); x += 1.25f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::SPACE), Position(x + 2.375f, y)); x += 5.75f; // 5.75 units wide
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::RIGHT_ALT), Position(x, y)); x += 1.25f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::RIGHT_WINDOWS), Position(x, y)); x += 1.25f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::CONTEXT_MENU), Position(x, y)); x += 1.25f;
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::RIGHT_CONTROL), Position(x + 0.25f, y));

    // --- Right-hand Clusters (Navigation and Numpad) ---
    const float nav_cluster_x = 15.0f;
    const float numpad_x = 18.5f;

    // System keys (above nav cluster)
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::PRINT_SCREEN), Position(nav_cluster_x, 0.0f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::SCROLL_LOCK), Position(nav_cluster_x + 1.0f, 0.0f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::PAUSE_BREAK), Position(nav_cluster_x + 2.0f, 0.0f));

    // Nav cluster
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::INSERT), Position(nav_cluster_x, 1.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::HOME), Position(nav_cluster_x + 1.0f, 1.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::PAGE_UP), Position(nav_cluster_x + 2.0f, 1.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::DELETE_KEY), Position(nav_cluster_x, 2.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::END), Position(nav_cluster_x + 1.0f, 2.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::PAGE_DOWN), Position(nav_cluster_x + 2.0f, 2.25f));

    // Arrow keys
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::ARROW_UP), Position(nav_cluster_x + 1.0f, 4.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::ARROW_LEFT), Position(nav_cluster_x, 5.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::ARROW_DOWN), Position(nav_cluster_x + 1.0f, 5.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::ARROW_RIGHT), Position(nav_cluster_x + 2.0f, 5.25f));

    // Numpad
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUM_LOCK), Position(numpad_x, 1.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUMPAD_DIVIDE), Position(numpad_x + 1.0f, 1.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUMPAD_MULTIPLY), Position(numpad_x + 2.0f, 1.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUMPAD_SUBTRACT), Position(numpad_x + 3.0f, 1.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUMPAD_7), Position(numpad_x, 2.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUMPAD_8), Position(numpad_x + 1.0f, 2.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUMPAD_9), Position(numpad_x + 2.0f, 2.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUMPAD_ADD), Position(numpad_x + 3.0f, 2.75f)); // Spans two rows
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUMPAD_4), Position(numpad_x, 3.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUMPAD_5), Position(numpad_x + 1.0f, 3.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUMPAD_6), Position(numpad_x + 2.0f, 3.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUMPAD_1), Position(numpad_x, 4.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUMPAD_2), Position(numpad_x + 1.0f, 4.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUMPAD_3), Position(numpad_x + 2.0f, 4.25f));
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUMPAD_ENTER), Position(numpad_x + 3.0f, 4.75f)); // Spans two rows
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUMPAD_0), Position(numpad_x + 0.5f, 5.25f)); // 2.0 units wide
    keys_.emplace_back(static_cast<uint16_t>(KeyCode::NUMPAD_DECIMAL), Position(numpad_x + 2.0f, 5.25f));
}