#pragma once

#pragma once
#include "Core/Keyboard/Key.h"
#include "Core/Keyboard/KeyCodes.h"
#include <cstdint>
#include <vector>

class Keyboard {
public:
    Keyboard();

    /**
     * @brief Gets a read-only collection of all keys on the keyboard.
     * @return A const reference to the vector of keys.
     */
    const std::vector<Key>& getKeys() const;

    /**
     * @brief Finds a specific key by its unique KeyCode.
     * This is the const version, which returns a pointer to a const Key.
     * @param id The KeyCode of the key to find.
     * @return A const pointer to the Key if found, otherwise nullptr.
     */
    const Key* findKeyById(KeyCode id) const {
        auto it = std::find_if(keys_.begin(), keys_.end(), [id](const Key& key) {
            // Cast both sides to the same underlying integer type for a clean comparison.
            return key.getId() == static_cast<uint16_t>(id);
            });
        return (it != keys_.end()) ? &(*it) : nullptr;
    }

    /**
     * @brief Finds a specific key by its unique KeyCode.
     * This is the non-const version, which returns a pointer to a mutable Key.
     * @param id The KeyCode of the key to find.
     * @return A pointer to the Key if found, otherwise nullptr.
     */
    Key* findKeyById(KeyCode id);

private:
    void initializeLayout();
    void buildNeighborMaps();

    std::vector<Key> keys_;
};