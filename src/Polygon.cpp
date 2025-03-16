#include "geometry/Polygon.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <stack>

Polygon::Polygon(const std::vector<Point>& vertices) : vertices(vertices) {}

void Polygon::add_vertex(const Point& point) {
    vertices.push_back(point);
}

float Polygon::area() const noexcept {
    if (vertices.size() < 3) {
        return 0.0f;
    }
    
    float sum = 0.0f;
    for (size_t i = 0; i < vertices.size(); ++i) {
        const Point& current = vertices[i];
        const Point& next = vertices[(i + 1) % vertices.size()];
        
        // 使用叉积计算有符号面积
        sum += (current.x * next.y - next.x * current.y);
    }
    
    // 取绝对值并除以2
    return std::abs(sum) * 0.5f;
}

bool Polygon::contains_point(const Point& point, bool include_boundary) const noexcept {
    if (vertices.size() < 3) {
        return false;
    }
    
    // 射线法判断点是否在多边形内部
    bool inside = false;
    for (size_t i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++) {
        const Point& vi = vertices[i];
        const Point& vj = vertices[j];
        
        // 检查点是否在边界上
        if (include_boundary) {
            Line edge(vi, vj);
            if (edge.contains(point)) {
                return true;
            }
        }
        
        // 射线法：检查从点向右发射的射线与多边形边的交点数
        if (((vi.y > point.y) != (vj.y > point.y)) &&
            (point.x < (vj.x - vi.x) * (point.y - vi.y) / (vj.y - vi.y) + vi.x)) {
            inside = !inside;
        }
    }
    
    return inside;
}

float Polygon::perimeter() const noexcept {
    if (vertices.size() < 2) {
        return 0.0f;
    }
    
    float sum = 0.0f;
    for (size_t i = 0; i < vertices.size(); ++i) {
        const Point& current = vertices[i];
        const Point& next = vertices[(i + 1) % vertices.size()];
        
        sum += static_cast<float>(current.distance_to(next));
    }
    
    return sum;
}

Point Polygon::centroid() const {
    if (vertices.empty()) {
        throw std::runtime_error("Cannot compute centroid of empty polygon");
    }
    
    if (vertices.size() == 1) {
        return vertices[0];
    }
    
    if (vertices.size() == 2) {
        return Point(
            (vertices[0].x + vertices[1].x) * 0.5f,
            (vertices[0].y + vertices[1].y) * 0.5f,
            (vertices[0].z + vertices[1].z) * 0.5f
        );
    }
    
    // 对于多边形，使用加权平均计算重心
    float area_sum = 0.0f;
    Point centroid(0.0f, 0.0f, 0.0f);
    
    for (size_t i = 0; i < vertices.size(); ++i) {
        const Point& current = vertices[i];
        const Point& next = vertices[(i + 1) % vertices.size()];
        
        // 计算三角形的面积
        float area = (current.x * next.y - next.x * current.y) * 0.5f;
        area_sum += area;
        
        // 计算三角形的重心
        centroid.x += (current.x + next.x) * area / 3.0f;
        centroid.y += (current.y + next.y) * area / 3.0f;
    }
    
    if (std::abs(area_sum) < 1e-6f) {
        // 退化情况，使用顶点的平均值
        for (const auto& v : vertices) {
            centroid.x += v.x / static_cast<float>(vertices.size());
            centroid.y += v.y / static_cast<float>(vertices.size());
            centroid.z += v.z / static_cast<float>(vertices.size());
        }
    } else {
        centroid.x /= area_sum;
        centroid.y /= area_sum;
    }
    
    return centroid;
}

bool Polygon::is_convex() const noexcept {
    if (vertices.size() < 3) {
        return false;
    }
    
    bool sign = false;
    bool sign_set = false;
    
    for (size_t i = 0; i < vertices.size(); ++i) {
        const Point& p1 = vertices[i];
        const Point& p2 = vertices[(i + 1) % vertices.size()];
        const Point& p3 = vertices[(i + 2) % vertices.size()];
        
        // 计算叉积的z分量
        float cross_z = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
        
        // 第一次设置符号
        if (!sign_set) {
            sign = cross_z > 0;
            sign_set = true;
        } else if ((cross_z > 0) != sign) {
            // 如果符号不一致，则不是凸多边形
            return false;
        }
    }
    
    return true;
}

// Graham扫描算法计算凸包
Polygon Polygon::convex_hull() const {
    if (vertices.size() < 3) {
        return *this;
    }
    
    // 找到y坐标最小的点（如果有多个，取x最小的）
    size_t min_idx = 0;
    for (size_t i = 1; i < vertices.size(); ++i) {
        if (vertices[i].y < vertices[min_idx].y ||
            (vertices[i].y == vertices[min_idx].y && vertices[i].x < vertices[min_idx].x)) {
            min_idx = i;
        }
    }
    
    // 复制顶点并将最低点放在第一位
    std::vector<Point> points = vertices;
    std::swap(points[0], points[min_idx]);
    
    // 按极角排序其余点
    Point p0 = points[0];
    std::sort(points.begin() + 1, points.end(), [&p0](const Point& a, const Point& b) {
        // 计算极角
        float angle_a = std::atan2(a.y - p0.y, a.x - p0.x);
        float angle_b = std::atan2(b.y - p0.y, b.x - p0.x);
        
        if (std::abs(angle_a - angle_b) < 1e-6f) {
            // 如果极角相同，按距离排序
            return p0.distance_to(a) < p0.distance_to(b);
        }
        
        return angle_a < angle_b;
    });
    
    // Graham扫描
    std::vector<Point> hull;
    hull.push_back(points[0]);
    hull.push_back(points[1]);
    
    for (size_t i = 2; i < points.size(); ++i) {
        while (hull.size() > 1) {
            Point p1 = hull[hull.size() - 2];
            Point p2 = hull[hull.size() - 1];
            Point p3 = points[i];
            
            // 计算叉积的z分量
            float cross_z = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
            
            // 如果不是左转，则弹出栈顶
            if (cross_z <= 0) {
                hull.pop_back();
            } else {
                break;
            }
        }
        
        hull.push_back(points[i]);
    }
    
    return Polygon(hull);
}

float Polygon::distance_to(const Point& point) const noexcept {
    if (contains_point(point, true)) {
        return 0.0f;
    }
    
    float min_distance = std::numeric_limits<float>::max();
    
    // 计算点到所有边的最短距离
    for (size_t i = 0; i < vertices.size(); ++i) {
        const Point& v1 = vertices[i];
        const Point& v2 = vertices[(i + 1) % vertices.size()];
        
        Line edge(v1, v2);
        float distance = edge.distance_to(point);
        
        min_distance = std::min(min_distance, distance);
    }
    
    return min_distance;
}

bool Polygon::intersects(const Polygon& other) const noexcept {
    // 检查是否有任何边相交
    auto this_edges = edges();
    auto other_edges = other.edges();
    
    for (const auto& edge1 : this_edges) {
        for (const auto& edge2 : other_edges) {
            if (edge1.intersects(edge2)) {
                return true;
            }
        }
    }
    
    // 检查一个多边形是否包含另一个多边形的任何顶点
    for (const auto& v : vertices) {
        if (other.contains_point(v)) {
            return true;
        }
    }
    
    for (const auto& v : other.vertices) {
        if (contains_point(v)) {
            return true;
        }
    }
    
    return false;
}

std::pair<Point, Point> Polygon::bounding_box() const noexcept {
    if (vertices.empty()) {
        return {Point(0, 0, 0), Point(0, 0, 0)};
    }
    
    Point min = vertices[0];
    Point max = vertices[0];
    
    for (const auto& v : vertices) {
        min.x = std::min(min.x, v.x);
        min.y = std::min(min.y, v.y);
        min.z = std::min(min.z, v.z);
        
        max.x = std::max(max.x, v.x);
        max.y = std::max(max.y, v.y);
        max.z = std::max(max.z, v.z);
    }
    
    return {min, max};
}

Polygon Polygon::simplify(float epsilon) const {
    if (vertices.size() < 3) {
        return *this;
    }
    
    std::vector<Point> simplified;
    simplified.push_back(vertices[0]);
    
    for (size_t i = 1; i < vertices.size() - 1; ++i) {
        const Point& prev = vertices[i - 1];
        const Point& curr = vertices[i];
        const Point& next = vertices[i + 1];
        
        if (!Line::are_collinear(prev, curr, next, epsilon)) {
            simplified.push_back(curr);
        }
    }
    
    // 添加最后一个点
    simplified.push_back(vertices.back());
    
    return Polygon(simplified);
}

std::vector<Line> Polygon::edges() const {
    std::vector<Line> result;
    
    if (vertices.size() < 2) {
        return result;
    }
    
    result.reserve(vertices.size());
    
    for (size_t i = 0; i < vertices.size(); ++i) {
        const Point& current = vertices[i];
        const Point& next = vertices[(i + 1) % vertices.size()];
        
        result.emplace_back(current, next);
    }
    
    return result;
}

std::ostream& operator<<(std::ostream& os, const Polygon& polygon) {
    os << "Polygon[";
    for (size_t i = 0; i < polygon.vertices.size(); ++i) {
        os << polygon.vertices[i];
        if (i < polygon.vertices.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}
