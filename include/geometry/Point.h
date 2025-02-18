#pragma once

#include <iostream>
#include <cmath>
#include <stdexcept>

class Point
{
public:
    float x, y, z;
    constexpr explicit Point(float x = 0.0f, float y = 0.0f, float z = 0.0f) noexcept : x(x), y(y), z(z) {}

    [[nodiscard]]
    double distance_to(const Point &other) const noexcept;

    [[nodiscard]]
    double magnitude() const noexcept;

    [[nodiscard]]
    Point normalized() const noexcept;
    // Arithmetic operators
    Point &operator+=(const Point &rhs) noexcept;
    Point &operator-=(const Point &rhs) noexcept;
    Point &operator*=(double scalar) noexcept;
    Point &operator/=(double scalar);

    [[nodiscard]] constexpr float magnitude_squared() const noexcept {
        return x*x + y*y + z*z;
    }
};

// Stream operator
std::ostream &operator<<(std::ostream &os, const Point &p);

// Arithmetic operators
[[nodiscard]] constexpr Point operator+(Point lhs, const Point &rhs) noexcept;
[[nodiscard]] constexpr Point operator-(Point lhs, const Point &rhs) noexcept;
[[nodiscard]] constexpr Point operator*(Point p, double scalar) noexcept;
[[nodiscard]] constexpr Point operator*(double scalar, Point p) noexcept;
[[nodiscard]] Point operator/(Point p, double scalar);

// Comparison operators
[[nodiscard]] constexpr bool operator==(const Point &lhs, const Point &rhs) noexcept;
[[nodiscard]] constexpr bool operator!=(const Point &lhs, const Point &rhs) noexcept;

// Vector operations
[[nodiscard]] constexpr double dot_product(const Point &a, const Point &b) noexcept;
[[nodiscard]] constexpr Point cross_product(const Point &a, const Point &b) noexcept;

// ===== Implementation =====
inline double Point::distance_to(const Point &other) const noexcept
{
    const double dx = x - other.x;
    const double dy = y - other.y;
    const double dz = z - other.z;
    return std::hypot(dx, dy, dz);
}

inline double Point::magnitude() const noexcept
{
    return std::hypot(x, y, z);
}

inline Point Point::normalized() const
{
    const double len = magnitude();
    if (len == 0.0)
    {
        throw std::runtime_error("Cannot normalize zero-length vector");
    }
    return *this / len;
}

inline Point &Point::operator+=(const Point &rhs) noexcept
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

inline Point &Point::operator-=(const Point &rhs) noexcept
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

inline Point &Point::operator*=(double scalar) noexcept
{
    x = static_cast<float>(x * scalar);
    y = static_cast<float>(y * scalar);
    z = static_cast<float>(z * scalar);
    return *this;
}

inline Point &Point::operator/=(double scalar)
{
    if (scalar == 0.0)
    {
        throw std::runtime_error("Division by zero in Point::operator/=");
    }
    return *this *= (1.0 / scalar);
}

constexpr Point operator+(Point lhs, const Point &rhs) noexcept
{
    return lhs += rhs;
}

constexpr Point operator-(Point lhs, const Point &rhs) noexcept
{
    return lhs -= rhs;
}

constexpr Point operator*(Point p, double scalar) noexcept
{
    return p *= scalar;
}

constexpr Point operator*(double scalar, Point p) noexcept
{
    return p *= scalar;
}

inline Point operator/(Point p, double scalar)
{
    return p /= scalar;
}

constexpr bool operator==(const Point &lhs, const Point &rhs) noexcept
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

constexpr bool operator!=(const Point &lhs, const Point &rhs) noexcept
{
    return !(lhs == rhs);
}

constexpr double dot_product(const Point &a, const Point &b) noexcept
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

constexpr Point cross_product(const Point &a, const Point &b) noexcept
{
    return Point{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x};
}

inline std::ostream &operator<<(std::ostream &os, const Point &p)
{
    os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    return os;
}
