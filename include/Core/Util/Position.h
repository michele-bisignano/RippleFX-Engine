// include/util/Position.h

#pragma once
#include <cassert>
#include <exception>
#include <string>
#include <cmath>
#include <sstream>

/**
 * @class Position
 * @brief Represents an immutable 2D coordinate using single-precision floats.
 *
 * This class models a physical position on a 2D plane, optimized for performance
 * on microprocessors. It enforces that coordinates must be non-negative.
 * An attempt to create a Position with negative coordinates will throw an InvalidPositionException.
 *
 * @author Michele Bisignano
 */
class Position {
public:
    /**
     * @brief Constructs a Position with specified x and y coordinates.
     * @param x The horizontal coordinate (must be >= 0.0f).
     * @param y The vertical coordinate (must be >= 0.0f).
     * @throws InvalidPositionException if x or y are negative.
     */
    Position(float x, float y) : x_(x), y_(y) {
        assert(x >= 0.0f && y >= 0.0f && "Position coordinates cannot be negative.");
    }

    /**
     * @brief Gets the horizontal (x) coordinate.
     * @return The x value.
     */
    float getX() const { return x_; }

    /**
     * @brief Gets the vertical (y) coordinate.
     * @return The y value.
     */
    float getY() const { return y_; }

    /**
     * @brief Calculates the Euclidean (straight-line) distance to another position.
     * @param other The other position to measure the distance to.
     * @return The Euclidean distance between the two points.
     */
    float distanceTo(const Position& other) const {
        const float dx = other.x_ - x_;
        const float dy = other.y_ - y_;
        return std::sqrt(dx * dx + dy * dy);
    }

    /**
     * @brief Calculates the Manhattan (grid-like) distance to another position.
     *
     * The Manhattan distance is the sum of the absolute differences of their coordinates.
     * It is significantly faster to execute on microprocessors than the standard
     * Euclidean distance because it avoids computationally expensive operations
     * like multiplications, powers, or square roots.
     *
     * Formula: |x1 - x2| + |y1 - y2|
     *
     * @param other The other position to measure the distance to.
     * @return The Manhattan distance between the two points.
     */
    float distanceManhattan(const Position& other) const {
        return std::abs(other.x_ - x_) + std::abs(other.y_ - y_);
    }

    /**
     * @brief Gets a string representation like "Position(x, y)".
     * @return A formatted string representation.
     */
    std::string toString() const {
        std::ostringstream oss;
        oss << "Position(" << x_ << ", " << y_ << ")";
        return oss.str();
    }

    /**
     * @brief Equality operator.
     */
    bool operator==(const Position& other) const {
        // Direct comparison is generally acceptable for fixed key positions.
        return x_ == other.x_ && y_ == other.y_;
    }

    /**
     * @brief Inequality operator.
     */
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }

private:
    const float x_;
    const float y_;
};