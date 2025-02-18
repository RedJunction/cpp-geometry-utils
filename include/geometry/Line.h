#pragma once

#include "Point.h"
#include <cmath>
#include <stdexcept>
#include <limits>
#include <algorithm>

class Line
{
public:
    Point start; ///< Starting point of the segment
    Point end;   ///< Ending point of the segment

    explicit constexpr Line(
        const Point &start = Point{},
        const Point &end = Point{}) noexcept : start(start), end(end) {}

    [[nodiscard]]
    float length() const noexcept;

    [[nodiscard]]
    bool intersects(const Line &other) const noexcept;

    [[nodiscard]]
    Point direction() const;

    [[nodiscard]]
    constexpr Point midpoint() const noexcept;
    [[nodiscard]]
    bool contains(const Point &point, float epsilon = 1e-6f) const noexcept;

    /**
     * @brief 计算点到线段的最短距离
     * @param point 目标点
     * @return 最短距离
     */
    [[nodiscard]]
    float distance_to(const Point &point) const noexcept;

    /**
     * @brief 计算点在直线上的投影点
     * @param point 目标点
     * @return 投影点坐标
     */
    [[nodiscard]]
    Point project(const Point &point) const noexcept;

    /**
     * @brief 计算点关于直线的对称点
     * @param point 目标点
     * @return 对称点坐标
     */
    [[nodiscard]]
    Point reflect(const Point &point) const noexcept;

    /**
     * @brief 计算两直线的夹角（弧度）
     * @param other 另一条直线
     * @return 夹角 [0, π/2]
     */
    [[nodiscard]]
    float angle_with(const Line &other) const noexcept;

    // 静态工具方法
    [[nodiscard]]
    static bool are_collinear(const Point &a, const Point &b, const Point &c,
                              float epsilon = 1e-6f) noexcept;

    // 贝塞尔曲线生成
    [[nodiscard]]
    static Point bezier_interpolate(const Point &p0, const Point &p1, float t) noexcept;

    [[nodiscard]]
    static Point bezier_interpolate(const Point &p0, const Point &p1,
                                    const Point &p2, float t) noexcept;

    [[nodiscard]]
    static Point bezier_interpolate(const Point &p0, const Point &p1,
                                    const Point &p2, const Point &p3, float t) noexcept;
};

// Comparison operators
[[nodiscard]] constexpr bool operator==(const Line &lhs, const Line &rhs) noexcept;
[[nodiscard]] constexpr bool operator!=(const Line &lhs, const Line &rhs) noexcept;

// Stream operator
std::ostream &operator<<(std::ostream &os, const Line &line);

// ===== Implementation =====

inline float Line::length() const noexcept
{
    return static_cast<float>(start.distance_to(end));
}

inline Point Line::direction() const
{
    const Point vec = end - start;
    return vec.normalized();
}

constexpr Point Line::midpoint() const noexcept
{
    return Point{
        (start.x + end.x) * 0.5f,
        (start.y + end.y) * 0.5f,
        (start.z + end.z) * 0.5f};
}

constexpr bool operator==(const Line &lhs, const Line &rhs) noexcept
{
    return (lhs.start == rhs.start) && (lhs.end == rhs.end);
}

constexpr bool operator!=(const Line &lhs, const Line &rhs) noexcept
{
    return !(lhs == rhs);
}

inline std::ostream &operator<<(std::ostream &os, const Line &line)
{
    os << "Line[" << line.start << " -> " << line.end << "]";
    return os;
}

inline bool Line::intersects(const Line &other) const noexcept
{
    // Implementation using cross product approach
    constexpr float epsilon = std::numeric_limits<float>::epsilon() * 1e6;

    const Point p1 = start;
    const Point p2 = end;
    const Point p3 = other.start;
    const Point p4 = other.end;

    // Calculate orientation values
    const auto ccw = [epsilon](const Point &a, const Point &b, const Point &c)
    {
        const float val = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
        return (val > epsilon) ? 1 : (val < -epsilon) ? -1
                                                      : 0;
    };

    const int o1 = ccw(p1, p2, p3);
    const int o2 = ccw(p1, p2, p4);
    const int o3 = ccw(p3, p4, p1);
    const int o4 = ccw(p3, p4, p2);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special cases (collinear points)
    const auto on_segment = [epsilon](const Point &a, const Point &b, const Point &c)
    {
        return (a.x <= std::max(b.x, c.x) + epsilon &&
                a.x >= std::min(b.x, c.x) - epsilon &&
                a.y <= std::max(b.y, c.y) + epsilon &&
                a.y >= std::min(b.y, c.y) - epsilon);
    };

    if (o1 == 0 && on_segment(p3, p1, p2))
        return true;
    if (o2 == 0 && on_segment(p4, p1, p2))
        return true;
    if (o3 == 0 && on_segment(p1, p3, p4))
        return true;
    if (o4 == 0 && on_segment(p2, p3, p4))
        return true;

    return false;
}

// ===== 实现 =====
inline bool Line::contains(const Point &p, float epsilon) const noexcept
{
    if (!are_collinear(start, end, p, epsilon))
        return false;

    const float min_x = std::min(start.x, end.x) - epsilon;
    const float max_x = std::max(start.x, end.x) + epsilon;
    const float min_y = std::min(start.y, end.y) - epsilon;
    const float max_y = std::max(start.y, end.y) + epsilon;
    const float min_z = std::min(start.z, end.z) - epsilon;
    const float max_z = std::max(start.z, end.z) + epsilon;

    return p.x >= min_x && p.x <= max_x &&
           p.y >= min_y && p.y <= max_y &&
           p.z >= min_z && p.z <= max_z;
}

inline float Line::distance_to(const Point &p) const noexcept
{
    const Point projection = project(p);
    return static_cast<float>(p.distance_to(projection));
}

inline Point Line::project(const Point &p) const noexcept
{
    const Point vec = end - start;
    const Point rel = p - start;
    const float t = static_cast<float>(dot_product(rel, vec) / vec.magnitude_squared());

    if (t <= 0.0f)
        return start;
    if (t >= 1.0f)
        return end;
    return start + vec * t;
}

inline Point Line::reflect(const Point &p) const noexcept
{
    const Point proj = project(p);
    return proj * 2.0f - p;
}

inline float Line::angle_with(const Line &other) const noexcept
{
    const Point dir1 = direction();
    const Point dir2 = other.direction();
    const float dot = static_cast<float>(dot_product(dir1, dir2));
    return std::acos(std::clamp(std::abs(dot), 0.0f, 1.0f));
}

inline bool Line::are_collinear(const Point &a, const Point &b,
                                const Point &c, float epsilon) noexcept
{
    const Point ab = b - a;
    const Point ac = c - a;
    return cross_product(ab, ac).magnitude() < epsilon;
}

// 贝塞尔曲线实现
inline Point Line::bezier_interpolate(const Point &p0, const Point &p1,
                                      float t) noexcept
{
    return p0 * (1.0f - t) + p1 * t;
}

inline Point Line::bezier_interpolate(const Point &p0, const Point &p1,
                                      const Point &p2, float t) noexcept
{
    const Point q0 = bezier_interpolate(p0, p1, t);
    const Point q1 = bezier_interpolate(p1, p2, t);
    return bezier_interpolate(q0, q1, t);
}

inline Point Line::bezier_interpolate(const Point &p0, const Point &p1,
                                      const Point &p2, const Point &p3,
                                      float t) noexcept
{
    const Point q0 = bezier_interpolate(p0, p1, t);
    const Point q1 = bezier_interpolate(p1, p2, t);
    const Point q2 = bezier_interpolate(p2, p3, t);
    return bezier_interpolate(q0, q1, q2, t);
}