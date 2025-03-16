#pragma once

#include "geometry/Point.h"
#include "geometry/Line.h"
#include "geometry/Plane.h"
#include "geometry/Polygon.h"
#include <vector>
#include <optional>
#include <cmath>

namespace geometry {
namespace utils {

/**
 * @brief 计算两点之间的距离
 * @param p1 第一个点
 * @param p2 第二个点
 * @return 距离
 */
[[nodiscard]] float distance(const Point& p1, const Point& p2) noexcept;

/**
 * @brief 计算点到直线的距离
 * @param point 点
 * @param line 直线
 * @return 距离
 */
[[nodiscard]] float distance(const Point& point, const Line& line) noexcept;

/**
 * @brief 计算点到平面的距离
 * @param point 点
 * @param plane 平面
 * @return 距离
 */
[[nodiscard]] float distance(const Point& point, const Plane& plane) noexcept;

/**
 * @brief 计算两条直线之间的最短距离
 * @param line1 第一条直线
 * @param line2 第二条直线
 * @return 最短距离
 */
[[nodiscard]] float distance(const Line& line1, const Line& line2) noexcept;

/**
 * @brief 计算直线与平面的交点
 * @param line 直线
 * @param plane 平面
 * @return 交点（如果不相交则返回std::nullopt）
 */
[[nodiscard]] std::optional<Point> intersection(const Line& line, const Plane& plane) noexcept;

/**
 * @brief 计算两条直线的交点
 * @param line1 第一条直线
 * @param line2 第二条直线
 * @return 交点（如果不相交则返回std::nullopt）
 */
[[nodiscard]] std::optional<Point> intersection(const Line& line1, const Line& line2) noexcept;

/**
 * @brief 计算两个平面的交线
 * @param plane1 第一个平面
 * @param plane2 第二个平面
 * @return 交线（如果平行则返回std::nullopt）
 */
[[nodiscard]] std::optional<Line> intersection(const Plane& plane1, const Plane& plane2) noexcept;

/**
 * @brief 计算三个平面的交点
 * @param plane1 第一个平面
 * @param plane2 第二个平面
 * @param plane3 第三个平面
 * @return 交点（如果不存在唯一交点则返回std::nullopt）
 */
[[nodiscard]] std::optional<Point> intersection(const Plane& plane1, const Plane& plane2, const Plane& plane3) noexcept;

/**
 * @brief 判断点是否在线段上
 * @param point 点
 * @param line 线段
 * @param epsilon 容差
 * @return 是否在线段上
 */
[[nodiscard]] bool is_point_on_line(const Point& point, const Line& line, float epsilon = 1e-6f) noexcept;

/**
 * @brief 判断点是否在平面上
 * @param point 点
 * @param plane 平面
 * @param epsilon 容差
 * @return 是否在平面上
 */
[[nodiscard]] bool is_point_on_plane(const Point& point, const Plane& plane, float epsilon = 1e-6f) noexcept;

/**
 * @brief 判断三点是否共线
 * @param p1 第一个点
 * @param p2 第二个点
 * @param p3 第三个点
 * @param epsilon 容差
 * @return 是否共线
 */
[[nodiscard]] bool are_collinear(const Point& p1, const Point& p2, const Point& p3, float epsilon = 1e-6f) noexcept;

/**
 * @brief 判断四点是否共面
 * @param p1 第一个点
 * @param p2 第二个点
 * @param p3 第三个点
 * @param p4 第四个点
 * @param epsilon 容差
 * @return 是否共面
 */
[[nodiscard]] bool are_coplanar(const Point& p1, const Point& p2, const Point& p3, const Point& p4, float epsilon = 1e-6f) noexcept;

/**
 * @brief 计算三角形面积
 * @param p1 第一个顶点
 * @param p2 第二个顶点
 * @param p3 第三个顶点
 * @return 面积
 */
[[nodiscard]] float triangle_area(const Point& p1, const Point& p2, const Point& p3) noexcept;

/**
 * @brief 计算四面体体积
 * @param p1 第一个顶点
 * @param p2 第二个顶点
 * @param p3 第三个顶点
 * @param p4 第四个顶点
 * @return 体积
 */
[[nodiscard]] float tetrahedron_volume(const Point& p1, const Point& p2, const Point& p3, const Point& p4) noexcept;

/**
 * @brief 计算点集的凸包
 * @param points 点集
 * @return 凸包多边形
 */
[[nodiscard]] Polygon convex_hull_2d(const std::vector<Point>& points);

/**
 * @brief 计算两个向量的夹角（弧度）
 * @param v1 第一个向量
 * @param v2 第二个向量
 * @return 夹角 [0, π]
 */
[[nodiscard]] float angle_between(const Point& v1, const Point& v2) noexcept;

/**
 * @brief 将角度从弧度转换为度
 * @param radians 弧度
 * @return 度
 */
[[nodiscard]] constexpr float radians_to_degrees(float radians) noexcept {
    return radians * 180.0f / static_cast<float>(M_PI);
}

/**
 * @brief 将角度从度转换为弧度
 * @param degrees 度
 * @return 弧度
 */
[[nodiscard]] constexpr float degrees_to_radians(float degrees) noexcept {
    return degrees * static_cast<float>(M_PI) / 180.0f;
}

} // namespace utils
} // namespace geometry
