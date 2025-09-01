/**
 * @author Michele Bisignano
 */
#pragma once

#include "Core/Effects/RippleEffect.h"
#include <array>
#include <vector>
#include <cstddef> // For std::byte

 // Define the maximum number of effects that can be active at once.
constexpr size_t MAX_ACTIVE_EFFECTS = 20;


/**
 * @brief Manages a pre-allocated memory pool for RippleEffect objects.
 * @author Michele Bisignano
 *
 * This class implements a memory management pattern known as a "Pool Allocator".
 * Its purpose is to eliminate real-time dynamic memory allocations (on the heap),
 * which can be slow and cause memory fragmentation, especially in
 * performance-critical applications like firmware or game engines.
 *
 * How it works:
 * 1. In the constructor, a single, large block of raw memory is allocated,
 *    sufficient to hold a predefined maximum number of RippleEffect objects.
 * 2. The `create()` method does not allocate new memory but uses "placement new"
 *    to construct a RippleEffect object in an already available memory slot
 *    within the pool.
 * 3. The `destroy()` method does not deallocate memory; instead, it explicitly calls
 *    the object's destructor and marks the memory slot as available again
 *    for future use.
 *
 * @note This implementation is not thread-safe.
 * @note The caller is responsible for calling `destroy()` for every object created
 *       with `create()`. The class returns raw pointers (`RippleEffect*`), and their
 *       lifecycle management depends on the correct use of the pool.
 * @see RippleEffect
 */
class EffectPool {
public:
    /**
     * @brief Constructs the memory pool, pre-allocating all necessary memory.
     */
    EffectPool();

    /**
     * @brief Creates a RippleEffect object within the pre-allocated pool.
     * @return A pointer to the new effect, or nullptr if the pool is full.
     */
    template<typename... Args>
    RippleEffect* create(Args&&... args);

    /**
     * @brief Returns an effect object's memory to the pool.
     * @param effect A pointer to the effect to be destroyed.
     */
    void destroy(RippleEffect* effect);

private:
    // A large block of raw memory to hold all our RippleEffect objects.
    std::array<std::byte, sizeof(RippleEffect)* MAX_ACTIVE_EFFECTS> memoryPool_;

    // A simple list to keep track of which "rooms" (pointers) are free.
    std::vector<RippleEffect*> freeSlots_;
};

// --- Template Implementation must be in the header file ---

/**
 * @author Michele Bisignano
 */
template<typename... Args>
RippleEffect* EffectPool::create(Args&&... args) {
    if (freeSlots_.empty()) {
        // No available "rooms" in our hotel.
        return nullptr;
    }

    // Get a free memory slot from the back of the list.
    RippleEffect* slot = freeSlots_.back();
    freeSlots_.pop_back();

    // Use "placement new" to construct a RippleEffect object directly in that memory slot.
    // This does NOT allocate new memory; it just calls the constructor.
    new (slot) RippleEffect(std::forward<Args>(args)...);

    return slot;
}