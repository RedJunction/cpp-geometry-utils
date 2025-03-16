#include "geometry/Plane.h"
#include <stdexcept>
#include <cmath>
#include <algorithm>

Plane::Plane(const Point& normal, const Point& point) 
    : point(point) {
    // 确保法向量是单位向量
    float magnitude = static_cast<float>(normal.magnitude());
    if (magnitude < 1e-6f) {
        throw std::invalid_argument("Normal vector cannot be zero");
    }
    this->normal = normal / magnitude;
}

Plane::Plane(const Point& p1, const Point& p2, const Point& p3) {
    // 计算两个向量
    Point v1 = p2 - p1;
    Point v2 = p3 - p1;
    
    // 计算法向量（两个向量的叉积）
    Point n = cross_product(v1, v2);
    
    float magnitude = static_cast<float>(n.magnitude());
    if (magnitude < 1e-6f) {
        throw std::invalid_argument("Points are collinear, cannot form a plane");
    }
    
    // 存储单位法向量和平面上的点
    normal = n / magnitude;
    point = p1;
}

Plane::Plane(float a, float b, float c, float d) {
    Point n(a, b, c);
    float magnitude = static_cast<float>(n.magnitude());
    
    if (magnitude < 1e-6f) {
        throw std::invalid_argument("Normal vector cannot be zero");
    }
    
    normal = n / magnitude;
    
    // 找平面上的一点
    // 选择非零系数对应的坐标轴
    if (std::abs(a) >= std::abs(b) && std::abs(a) >= std::abs(c) && std::abs(a) > 1e-6f) {
        point = Point(-d/a, 0, 0);
    } else if (std::abs(b) >= std::abs(a) && std::abs(b) >= std::abs(c) && std::abs(b) > 1e-6f) {
        point = Point(0, -d/b, 0);
    } else {
        point = Point(0, 0, -d/c);
    }
}

float Plane::d() const noexcept {
    return -static_cast<float>(dot_product(normal, point));
}

float Plane::signed_distance_to(const Point& p) const noexcept {
    return static_cast<float>(dot_product(normal, p)) + d();
}

float Plane::distance_to(const Point& p) const noexcept {
    return std::abs(signed_distance_to(p));
}

bool Plane::intersects(const Line& line) const noexcept {
    // 计算直线方向与平面法向量的点积
    Point direction = line.end - line.start;
    float dot = static_cast<float>(dot_product(normal, direction));
    
    // 如果点积接近于零，则直线与平面平行或在平面内
    if (std::abs(dot) < 1e-6f) {
        // 检查直线是否在平面内
        return std::abs(signed_distance_to(line.start)) < 1e-6f;
    }
    
    // 否则，直线与平面相交
    return true;
}

std::optional<Point> Plane::intersection_with(const Line& line) const noexcept {
    Point direction = line.end - line.start;
    float dot = static_cast<float>(dot_product(normal, direction));
    
    // 如果点积接近于零，则直线与平面平行或在平面内
    if (std::abs(dot) < 1e-6f) {
        return std::nullopt;
    }
    
    // 计算参数 t，使得 line.start + t * direction 在平面上
    float t = -(signed_distance_to(line.start)) / dot;
    
    // 计算交点
    return line.start + direction * t;
}

Point Plane::project(const Point& p) const noexcept {
    // 计算点到平面的有符号距离
    float dist = signed_distance_to(p);
    
    // 沿法向量方向移动点
    return p - normal * dist;
}

Point Plane::reflect(const Point& p) const noexcept {
    // 计算点到平面的有符号距离
    float dist = signed_distance_to(p);
    
    // 沿法向量方向移动点的两倍距离
    return p - normal * (2 * dist);
}

float Plane::angle_with(const Plane& other) const noexcept {
    // 计算两个法向量的点积
    float dot = static_cast<float>(dot_product(normal, other.normal));
    
    // 计算夹角（取绝对值，因为我们只关心最小角度）
    return std::acos(std::clamp(std::abs(dot), 0.0f, 1.0f));
}

bool Plane::contains(const Point& p, float epsilon) const noexcept {
    // 点在平面上当且仅当点到平面的距离小于epsilon
    return distance_to(p) < epsilon;
}

bool Plane::is_parallel_to(const Plane& other, float epsilon) const noexcept {
    // 两平面平行当且仅当它们的法向量平行
    // 即法向量的叉积接近于零向量
    Point cross = cross_product(normal, other.normal);
    return cross.magnitude() < epsilon;
}

std::ostream& operator<<(std::ostream& os, const Plane& plane) {
    os << "Plane[normal=" << plane.normal 
       << ", point=" << plane.point 
       << ", d=" << plane.d() << "]";
    return os;
}
