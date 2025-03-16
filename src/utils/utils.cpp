#include "utils/utils.h"
#include <algorithm>
#include <limits>

namespace geometry {
namespace utils {

float distance(const Point& p1, const Point& p2) noexcept {
    return static_cast<float>(p1.distance_to(p2));
}

float distance(const Point& point, const Line& line) noexcept {
    return line.distance_to(point);
}

float distance(const Point& point, const Plane& plane) noexcept {
    return plane.distance_to(point);
}

float distance(const Line& line1, const Line& line2) noexcept {
    // 计算两条直线之间的最短距离
    Point dir1 = line1.direction();
    Point dir2 = line2.direction();
    
    // 计算两个方向向量的叉积
    Point cross = cross_product(dir1, dir2);
    float cross_magnitude = static_cast<float>(cross.magnitude());
    
    // 如果叉积接近于零，则直线平行或重合
    if (cross_magnitude < 1e-6f) {
        // 计算点到直线的距离
        return line1.distance_to(line2.start);
    }
    
    // 计算连接两条直线起点的向量
    Point connecting = line2.start - line1.start;
    
    // 计算混合积的绝对值，即两条直线之间的最短距离
    return std::abs(static_cast<float>(dot_product(connecting, cross))) / cross_magnitude;
}

std::optional<Point> intersection(const Line& line, const Plane& plane) noexcept {
    return plane.intersection_with(line);
}

std::optional<Point> intersection(const Line& line1, const Line& line2) noexcept {
    // 只处理2D情况（z=0）
    // 对于3D情况，两条直线通常不相交
    
    // 检查两条线是否平行
    Point dir1 = line1.direction();
    Point dir2 = line2.direction();
    
    float cross_z = dir1.x * dir2.y - dir1.y * dir2.x;
    
    // 如果叉积接近于零，则直线平行或重合
    if (std::abs(cross_z) < 1e-6f) {
        return std::nullopt;
    }
    
    // 求解参数方程
    // line1: p1 + t1 * dir1
    // line2: p2 + t2 * dir2
    
    Point p1 = line1.start;
    Point p2 = line2.start;
    
    float t1 = ((p2.x - p1.x) * dir2.y - (p2.y - p1.y) * dir2.x) / cross_z;
    
    // 计算交点
    return p1 + dir1 * t1;
}

std::optional<Line> intersection(const Plane& plane1, const Plane& plane2) noexcept {
    // 检查两个平面是否平行
    if (plane1.is_parallel_to(plane2)) {
        return std::nullopt;
    }
    
    // 计算交线的方向向量（两个平面法向量的叉积）
    Point direction = cross_product(plane1.normal, plane2.normal);
    
    // 归一化方向向量
    direction = direction.normalized();
    
    // 求解交线上的一点
    // 选择一个坐标轴，假设该轴上的坐标为0
    Point point;
    
    // 检查方向向量的分量，选择最小的分量对应的坐标轴
    if (std::abs(direction.x) <= std::abs(direction.y) && std::abs(direction.x) <= std::abs(direction.z)) {
        // 假设x = 0，求解y和z
        float a1 = plane1.normal.y, b1 = plane1.normal.z, c1 = -plane1.d() - plane1.normal.x * 0;
        float a2 = plane2.normal.y, b2 = plane2.normal.z, c2 = -plane2.d() - plane2.normal.x * 0;
        
        // 解线性方程组
        float det = a1 * b2 - a2 * b1;
        if (std::abs(det) < 1e-6f) {
            // 如果行列式接近于零，尝试另一个坐标轴
            point = Point(0, 0, 0);
        } else {
            float y = (c1 * b2 - c2 * b1) / det;
            float z = (a1 * c2 - a2 * c1) / det;
            point = Point(0, y, z);
        }
    } else if (std::abs(direction.y) <= std::abs(direction.x) && std::abs(direction.y) <= std::abs(direction.z)) {
        // 假设y = 0，求解x和z
        float a1 = plane1.normal.x, b1 = plane1.normal.z, c1 = -plane1.d() - plane1.normal.y * 0;
        float a2 = plane2.normal.x, b2 = plane2.normal.z, c2 = -plane2.d() - plane2.normal.y * 0;
        
        // 解线性方程组
        float det = a1 * b2 - a2 * b1;
        if (std::abs(det) < 1e-6f) {
            // 如果行列式接近于零，尝试另一个坐标轴
            point = Point(0, 0, 0);
        } else {
            float x = (c1 * b2 - c2 * b1) / det;
            float z = (a1 * c2 - a2 * c1) / det;
            point = Point(x, 0, z);
        }
    } else {
        // 假设z = 0，求解x和y
        float a1 = plane1.normal.x, b1 = plane1.normal.y, c1 = -plane1.d() - plane1.normal.z * 0;
        float a2 = plane2.normal.x, b2 = plane2.normal.y, c2 = -plane2.d() - plane2.normal.z * 0;
        
        // 解线性方程组
        float det = a1 * b2 - a2 * b1;
        if (std::abs(det) < 1e-6f) {
            // 如果行列式接近于零，尝试另一个坐标轴
            point = Point(0, 0, 0);
        } else {
            float x = (c1 * b2 - c2 * b1) / det;
            float y = (a1 * c2 - a2 * c1) / det;
            point = Point(x, y, 0);
        }
    }
    
    // 创建交线
    return Line(point, point + direction);
}

std::optional<Point> intersection(const Plane& plane1, const Plane& plane2, const Plane& plane3) noexcept {
    // 检查是否有两个平面平行
    if (plane1.is_parallel_to(plane2) || plane1.is_parallel_to(plane3) || plane2.is_parallel_to(plane3)) {
        return std::nullopt;
    }
    
    // 使用克莱姆法则求解线性方程组
    // a1*x + b1*y + c1*z + d1 = 0
    // a2*x + b2*y + c2*z + d2 = 0
    // a3*x + b3*y + c3*z + d3 = 0
    
    float a1 = plane1.normal.x, b1 = plane1.normal.y, c1 = plane1.normal.z, d1 = plane1.d();
    float a2 = plane2.normal.x, b2 = plane2.normal.y, c2 = plane2.normal.z, d2 = plane2.d();
    float a3 = plane3.normal.x, b3 = plane3.normal.y, c3 = plane3.normal.z, d3 = plane3.d();
    
    // 计算行列式
    float det = a1 * (b2 * c3 - b3 * c2) - b1 * (a2 * c3 - a3 * c2) + c1 * (a2 * b3 - a3 * b2);
    
    if (std::abs(det) < 1e-6f) {
        // 如果行列式接近于零，则三个平面不相交于一点
        return std::nullopt;
    }
    
    // 计算x, y, z
    float x = (-d1 * (b2 * c3 - b3 * c2) + b1 * (d2 * c3 - d3 * c2) - c1 * (d2 * b3 - d3 * b2)) / det;
    float y = (-a1 * (d2 * c3 - d3 * c2) + d1 * (a2 * c3 - a3 * c2) - c1 * (a2 * d3 - a3 * d2)) / det;
    float z = (-a1 * (b2 * d3 - b3 * d2) + b1 * (a2 * d3 - a3 * d2) - d1 * (a2 * b3 - a3 * b2)) / det;
    
    return Point(x, y, z);
}

bool is_point_on_line(const Point& point, const Line& line, float epsilon) noexcept {
    return line.contains(point, epsilon);
}

bool is_point_on_plane(const Point& point, const Plane& plane, float epsilon) noexcept {
    return plane.contains(point, epsilon);
}

bool are_collinear(const Point& p1, const Point& p2, const Point& p3, float epsilon) noexcept {
    return Line::are_collinear(p1, p2, p3, epsilon);
}

bool are_coplanar(const Point& p1, const Point& p2, const Point& p3, const Point& p4, float epsilon) noexcept {
    // 计算混合积
    Point v1 = p2 - p1;
    Point v2 = p3 - p1;
    Point v3 = p4 - p1;
    
    float mixed_product = static_cast<float>(dot_product(cross_product(v1, v2), v3));
    
    // 如果混合积接近于零，则四点共面
    return std::abs(mixed_product) < epsilon;
}

float triangle_area(const Point& p1, const Point& p2, const Point& p3) noexcept {
    // 使用叉积计算三角形面积
    Point v1 = p2 - p1;
    Point v2 = p3 - p1;
    
    return static_cast<float>(cross_product(v1, v2).magnitude()) * 0.5f;
}

float tetrahedron_volume(const Point& p1, const Point& p2, const Point& p3, const Point& p4) noexcept {
    // 计算混合积
    Point v1 = p2 - p1;
    Point v2 = p3 - p1;
    Point v3 = p4 - p1;
    
    float mixed_product = static_cast<float>(dot_product(cross_product(v1, v2), v3));
    
    // 四面体体积为混合积的绝对值除以6
    return std::abs(mixed_product) / 6.0f;
}

Polygon convex_hull_2d(const std::vector<Point>& points) {
    if (points.size() < 3) {
        return Polygon(points);
    }
    
    // 使用Polygon类中的凸包算法
    Polygon poly(points);
    return poly.convex_hull();
}

float angle_between(const Point& v1, const Point& v2) noexcept {
    float dot = static_cast<float>(dot_product(v1, v2));
    float mag1 = static_cast<float>(v1.magnitude());
    float mag2 = static_cast<float>(v2.magnitude());
    
    if (mag1 < 1e-6f || mag2 < 1e-6f) {
        return 0.0f;
    }
    
    // 计算夹角的余弦值
    float cos_angle = dot / (mag1 * mag2);
    
    // 确保余弦值在[-1, 1]范围内
    cos_angle = std::clamp(cos_angle, -1.0f, 1.0f);
    
    // 返回夹角（弧度）
    return std::acos(cos_angle);
}

} // namespace utils
} // namespace geometry
