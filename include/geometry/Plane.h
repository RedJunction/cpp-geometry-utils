#pragma once

#include "Point.h"
#include "Line.h"
#include <optional>

/**
 * @brief 表示3D空间中的平面
 * 
 * 平面由一个点和一个法向量定义，方程形式为: ax + by + cz + d = 0
 * 其中 (a,b,c) 是法向量，d = -(ax0 + by0 + cz0)，(x0,y0,z0) 是平面上的点
 */
class Plane {
public:
    Point point;     ///< 平面上的一个点
    Point normal;    ///< 平面的单位法向量

    /**
     * @brief 从法向量和平面上的一点构造平面
     * @param normal 法向量
     * @param point 平面上的点
     */
    Plane(const Point& normal, const Point& point);

    /**
     * @brief 从平面上的三个点构造平面
     * @param p1 第一个点
     * @param p2 第二个点
     * @param p3 第三个点
     * @throws std::invalid_argument 如果三点共线
     */
    Plane(const Point& p1, const Point& p2, const Point& p3);

    /**
     * @brief 从平面方程系数构造平面: ax + by + cz + d = 0
     * @param a x系数
     * @param b y系数
     * @param c z系数
     * @param d 常数项
     * @throws std::invalid_argument 如果法向量为零向量
     */
    Plane(float a, float b, float c, float d);

    /**
     * @brief 获取平面方程的常数项 d
     * @return 常数项 d，使得 ax + by + cz + d = 0
     */
    [[nodiscard]] float d() const noexcept;

    /**
     * @brief 计算点到平面的有符号距离
     * @param p 目标点
     * @return 有符号距离（正值表示点在法向量方向，负值表示点在法向量反方向）
     */
    [[nodiscard]] float signed_distance_to(const Point& p) const noexcept;

    /**
     * @brief 计算点到平面的距离
     * @param p 目标点
     * @return 距离（始终为非负值）
     */
    [[nodiscard]] float distance_to(const Point& p) const noexcept;

    /**
     * @brief 判断直线是否与平面相交
     * @param line 目标直线
     * @return 是否相交
     */
    [[nodiscard]] bool intersects(const Line& line) const noexcept;

    /**
     * @brief 计算直线与平面的交点
     * @param line 目标直线
     * @return 交点（如果不相交则返回std::nullopt）
     */
    [[nodiscard]] std::optional<Point> intersection_with(const Line& line) const noexcept;

    /**
     * @brief 计算点在平面上的投影
     * @param p 目标点
     * @return 投影点
     */
    [[nodiscard]] Point project(const Point& p) const noexcept;

    /**
     * @brief 计算点关于平面的对称点
     * @param p 目标点
     * @return 对称点
     */
    [[nodiscard]] Point reflect(const Point& p) const noexcept;

    /**
     * @brief 计算两平面的夹角（弧度）
     * @param other 另一个平面
     * @return 夹角 [0, π/2]
     */
    [[nodiscard]] float angle_with(const Plane& other) const noexcept;

    /**
     * @brief 判断点是否在平面上
     * @param p 目标点
     * @param epsilon 容差
     * @return 是否在平面上
     */
    [[nodiscard]] bool contains(const Point& p, float epsilon = 1e-6f) const noexcept;

    /**
     * @brief 判断两个平面是否平行
     * @param other 另一个平面
     * @param epsilon 容差
     * @return 是否平行
     */
    [[nodiscard]] bool is_parallel_to(const Plane& other, float epsilon = 1e-6f) const noexcept;
};

// Stream operator
std::ostream& operator<<(std::ostream& os, const Plane& plane);
