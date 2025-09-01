#pragma once

#pragma once
#include "Core/Keyboard/KeyCodes.h"
#include "Core/Util/Position.h"
#include <vector>

/**
 * @class Key
 * @brief Represents a single, immutable physical key on a keyboard.
 *
 * This class models a key by its fixed physical properties: a unique ID,
 * its position on the keyboard grid, and a pre-calculated list of its
 * immediate neighbors. It holds no dynamic state (like color).
 * 
 * @author Michele Bisignano
 */
class Key {
public:
    /**
     * @brief Constructs a Key with a specific identifier and position.
     * @param id A unique identifier for the key from the KeyCode enum.
     * @param position The physical coordinates of the key.
     */
    Key(uint16_t id, const Position& position);

    /**
     * @brief Gets the unique identifier of the key.
     * @return The key's ID.
     */
    uint16_t getId() const;

    /**
     * @brief Gets the physical position of the key.
     * @return A const reference to the key's Position object.
     */
    const Position& getPosition() const;

    /**
     * @brief Gets the index of this key within the Keyboard's main vector.
     * 
     * @return the key's index.
     */
    size_t getIndex() const;


    /**
     * @brief A list of pointers to this key's immediate neighbors.
     *
     * This list is calculated once by the Keyboard class upon initialization.
     * It is public for read-only access and is essential for propagation-based
     * effects (cellular automata). The pointers are to const Keys, ensuring
     * that neighbors cannot be modified.
     */
    std::vector<const Key*> neighbors;

private:
    friend class Keyboard; // Grant Keyboard permission to set the private index_
    const uint16_t id_;
    const Position position_;
    size_t index_; // The key's own index within the Keyboard's main vector.

};