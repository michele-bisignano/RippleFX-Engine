
#include "Core/Keyboard/Key.h"

Key::Key(uint16_t id, const Position& position)
    : id_(id), position_(position)
{
}

uint16_t Key::getId() const {
    return id_;
}

const Position& Key::getPosition() const {
    return position_;
}

size_t Key::getIndex() const {
    return index_;
}