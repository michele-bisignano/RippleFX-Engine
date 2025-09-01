// include/util/Color.h

#pragma once
#include <algorithm>
#include <iomanip>
#include <random>
#include <sstream>
#include <string>

/**
 * @class Color
 * @brief Represents an immutable RGB color, optimized for embedded systems.
 *
 * The Color class models a color using red, green, and blue components (0-255 each).
 * It provides methods to scale brightness, blend with another color, and lighten the color.
 *
 * @author Michele Bisignano
 */
class Color {
public:
    /**
     * @brief Constructs a Color object with specified RGB components.
     * @param red   Red component (0-255).
     * @param green Green component (0-255).
     * @param blue  Blue component (0-255).
     */
    Color(int red, int green, int blue)
        : red_(clamp(red)), green_(clamp(green)), blue_(clamp(blue)) {
    }

    /**
     * @brief Gets the red component.
     * @return Red value (0-255).
     */
    int getRed() const { return red_; }

    /**
     * @brief Gets the green component.
     * @return Green value (0-255).
     */
    int getGreen() const { return green_; }

    /**
     * @brief Gets the blue component.
     * @return Blue value (0-255).
     */
    int getBlue() const { return blue_; }

    /**
    * @brief Scales the color's brightness using fast integer math.
    *
    * This is a highly optimized function for firmware. It avoids floating-point
    * math and uses a fast bit-shift instead of a slow division.
    *
    * @param intensity The brightness factor (0=off, 255=full brightness).
    * @return A new, scaled Color object.
    */
    Color scale(uint8_t intensity) const {
        // The formula is (color * intensity) / 256.
        // Division by 256 is a simple bitwise right shift by 8.
        return Color(
            (red_ * intensity) >> 8,
            (green_ * intensity) >> 8,
            (blue_ * intensity) >> 8
        );
    }

    /**
     * @brief Generates a random color.
     * @warning This implementation uses std::mt19937 which can be heavy
     *          in terms of memory and performance for some microcontrollers.
     *          Consider a simpler pseudo-random generator (like an LCG) for firmware.
     * @return A Color object with random RGB values.
     */
    static Color randomColor() {
        static std::mt19937 rng(std::random_device{}());
        static std::uniform_int_distribution<int> dist(0, 255);
        return Color(dist(rng), dist(rng), dist(rng));
    }

    /**
     * @brief Lightens the color by a specified factor.
     * @param factor Lightening factor (0.0f - 1.0f).
     * @return A new Color with increased brightness.
     */
    Color lighten(float factor) const {
        factor = std::clamp(factor, 0.0f, 1.0f);
        return Color(
            static_cast<int>(red_ + (255 - red_) * factor),
            static_cast<int>(green_ + (255 - green_) * factor),
            static_cast<int>(blue_ + (255 - blue_) * factor)
        );
    }

    /**
     * @brief Blends this color with another using weighted interpolation.
     * @param other The other Color to blend with.
     * @param factor Interpolation factor (0.0f - 1.0f).
     * @return A new Color resulting from the weighted blend.
     */
    Color blend(const Color& other, float factor) const {
        factor = std::clamp(factor, 0.0f, 1.0f);
        return Color(
            static_cast<int>(red_ + (other.red_ - red_) * factor),
            static_cast<int>(green_ + (other.green_ - green_) * factor),
            static_cast<int>(blue_ + (other.blue_ - blue_) * factor)
        );
    }

    /**
    * @brief Additively blends this color with another.
    *
    * Each component is added together, clamping at 255. This is useful for
    * layering light effects, where multiple lights make a surface brighter.
    * @param other The color to add to this one.
    * @return A new Color representing the sum of both colors.
    */
    Color add(const Color& other) const {
        return Color(
            std::min(255, red_ + other.getRed()),
            std::min(255, green_ + other.getGreen()),
            std::min(255, blue_ + other.getBlue())
        );
    }

    /**
     * @brief Converts the color to a hexadecimal string ("#RRGGBB").
     * @warning This function uses std::ostringstream, which relies on dynamic
     *          memory allocation (heap). This should be avoided in firmware.
     *          A safer alternative would use a fixed-size buffer and snprintf.
     * @return String in hex format.
     */
    std::string toHex() const {
        std::ostringstream oss;
        oss << "#" << std::uppercase << std::setfill('0') << std::hex
            << std::setw(2) << red_
            << std::setw(2) << green_
            << std::setw(2) << blue_;
        return oss.str();
    }

    /**
     * @brief Equality operator.
     */
    bool operator==(const Color& other) const {
        return red_ == other.red_ && green_ == other.green_ && blue_ == other.blue_;
    }

    /**
     * @brief Inequality operator.
     */
    bool operator!=(const Color& other) const {
        return !(*this == other);
    }

private:
    int red_;
    int green_;
    int blue_;

    /**
     * @brief Clamps a value between 0 and 255.
     */
    static int clamp(int value) {
        return std::max(0, std::min(255, value));
    }
};