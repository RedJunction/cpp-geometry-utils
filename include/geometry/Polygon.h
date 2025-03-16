#pragma once

#include "Point.h"
#include "Line.h"
#include <vector>
#include <optional>

/**
 * @brief 表示2D多边形
 * 
 * 提供多边形的基本操作，如面积计算、点包含测试、凸包计算等
 */
class Polygon {
public:
    std::vector<Point> vertices;  ///< 多边形的顶点（按顺序存储）

    /**
     * @brief 默认构造函数
     */
    Polygon() = default;

    /**
     * @brief 从顶点列表构造多边形
     * @param vertices 顶点列表
     */
    explicit Polygon(const std::vector<Point>& vertices);

    /**
     * @brief 添加顶点到多边形
     * @param point 新顶点
     */
    void add_vertex(const Point& point);

    /**
     * @brief 计算多边形的面积
     * @return 面积（非负值）
     * @note 对于非简单多边形，返回的是有符号面积的绝对值
     */
    [[nodiscard]] float area() const noexcept;

    /**
     * @brief 判断点是否在多边形内部
     * @param point 目标点
     * @param include_boundary 是否包含边界
     * @return 是否在内部
     */
    [[nodiscard]] bool contains_point(const Point& point, bool include_boundary = true) const noexcept;

    /**
     * @brief 计算多边形的周长
     * @return 周长
     */
    [[nodiscard]] float perimeter() const noexcept;

    /**
     * @brief 计算多边形的重心
     * @return 重心坐标
     */
    [[nodiscard]] Point centroid() const;

    /**
     * @brief 判断多边形是否为凸多边形
     * @return 是否为凸多边形
     */
    [[nodiscard]] bool is_convex() const noexcept;

    /**
     * @brief 计算多边形的凸包
     * @return 凸包多边形
     */
    [[nodiscard]] Polygon convex_hull() const;

    /**
     * @brief 计算点到多边形的最短距离
     * @param point 目标点
     * @return 最短距离（点在多边形内部时返回0）
     */
    [[nodiscard]] float distance_to(const Point& point) const noexcept;

    /**
     * @brief 判断多边形是否与另一个多边形相交
     * @param other 另一个多边形
     * @return 是否相交
     */
    [[nodiscard]] bool intersects(const Polygon& other) const noexcept;

    /**
     * @brief 计算多边形的边界框
     * @return 边界框的左下角和右上角坐标
     */
    [[nodiscard]] std::pair<Point, Point> bounding_box() const noexcept;

    /**
     * @brief 简化多边形（移除共线点）
     * @param epsilon 容差
     * @return 简化后的多边形
     */
    [[nodiscard]] Polygon simplify(float epsilon = 1e-6f) const;

    /**
     * @brief 获取多边形的边
     * @return 边的列表
     */
    [[nodiscard]] std::vector<Line> edges() const;
};

// Stream operator
std::ostream& operator<<(std::ostream& os, const Polygon& polygon);
