#include <iostream>
#include <vector>
#include <iomanip>
#include "geometry/Point.h"
#include "geometry/Line.h"
#include "geometry/Plane.h"
#include "geometry/Polygon.h"
#include "utils/utils.h"

// 辅助函数：打印分隔线
void print_separator(const std::string& title = "") {
    std::cout << "\n" << std::string(50, '-') << "\n";
    if (!title.empty()) {
        std::cout << "  " << title << "\n";
        std::cout << std::string(50, '-') << "\n";
    }
}

// 演示点操作
void demo_points() {
    print_separator("点操作演示");
    
    // 创建点
    Point p1(1.0f, 2.0f, 3.0f);
    Point p2(4.0f, 5.0f, 6.0f);
    
    std::cout << "p1 = " << p1 << std::endl;
    std::cout << "p2 = " << p2 << std::endl;
    
    // 点的运算
    std::cout << "p1 + p2 = " << (p1 + p2) << std::endl;
    std::cout << "p2 - p1 = " << (p2 - p1) << std::endl;
    std::cout << "p1 * 2.5 = " << (p1 * 2.5) << std::endl;
    std::cout << "p2 / 2.0 = " << (p2 / 2.0) << std::endl;
    
    // 点的属性
    std::cout << "p1的模长 = " << p1.magnitude() << std::endl;
    std::cout << "p1的单位向量 = " << p1.normalized() << std::endl;
    std::cout << "p1到p2的距离 = " << p1.distance_to(p2) << std::endl;
    
    // 向量运算
    std::cout << "p1 · p2 (点积) = " << dot_product(p1, p2) << std::endl;
    std::cout << "p1 × p2 (叉积) = " << cross_product(p1, p2) << std::endl;
}

// 演示线段操作
void demo_lines() {
    print_separator("线段操作演示");
    
    // 创建线段
    Point p1(0.0f, 0.0f, 0.0f);
    Point p2(3.0f, 4.0f, 0.0f);
    Line line1(p1, p2);
    
    Point p3(1.0f, 1.0f, 0.0f);
    Point p4(4.0f, 2.0f, 0.0f);
    Line line2(p3, p4);
    
    std::cout << "line1 = " << line1 << std::endl;
    std::cout << "line2 = " << line2 << std::endl;
    
    // 线段属性
    std::cout << "line1的长度 = " << line1.length() << std::endl;
    std::cout << "line1的方向向量 = " << line1.direction() << std::endl;
    std::cout << "line1的中点 = " << line1.midpoint() << std::endl;
    
    // 点与线段的关系
    Point test_point(1.0f, 2.0f, 0.0f);
    std::cout << "测试点 = " << test_point << std::endl;
    std::cout << "测试点到line1的距离 = " << line1.distance_to(test_point) << std::endl;
    std::cout << "测试点在line1上的投影 = " << line1.project(test_point) << std::endl;
    std::cout << "测试点关于line1的对称点 = " << line1.reflect(test_point) << std::endl;
    
    // 线段之间的关系
    std::cout << "line1和line2是否相交: " << (line1.intersects(line2) ? "是" : "否") << std::endl;
    std::cout << "line1和line2的夹角 = " << line1.angle_with(line2) << " 弧度" << std::endl;
    std::cout << "line1和line2的夹角 = " << geometry::utils::radians_to_degrees(line1.angle_with(line2)) << " 度" << std::endl;
    
    // 贝塞尔曲线
    std::cout << "二阶贝塞尔曲线示例:" << std::endl;
    Point p0(0.0f, 0.0f, 0.0f);
    Point control(1.5f, 2.0f, 0.0f);
    Point p5(3.0f, 0.0f, 0.0f);
    
    for (int i = 0; i <= 5; ++i) {
        float t = i / 5.0f;
        Point bezier = Line::bezier_interpolate(p0, control, p5, t);
        std::cout << "t = " << t << ": " << bezier << std::endl;
    }
}

// 演示平面操作
void demo_planes() {
    print_separator("平面操作演示");
    
    // 创建平面
    Point p1(0.0f, 0.0f, 0.0f);
    Point p2(1.0f, 0.0f, 0.0f);
    Point p3(0.0f, 1.0f, 0.0f);
    Plane plane1(p1, p2, p3);
    
    Point normal(0.0f, 0.0f, 1.0f);
    Point point(0.0f, 0.0f, 5.0f);
    Plane plane2(normal, point);
    
    std::cout << "plane1 = " << plane1 << std::endl;
    std::cout << "plane2 = " << plane2 << std::endl;
    
    // 平面属性
    std::cout << "plane1的常数项d = " << plane1.d() << std::endl;
    
    // 点与平面的关系
    Point test_point(1.0f, 1.0f, 2.0f);
    std::cout << "测试点 = " << test_point << std::endl;
    std::cout << "测试点到plane1的有符号距离 = " << plane1.signed_distance_to(test_point) << std::endl;
    std::cout << "测试点到plane1的距离 = " << plane1.distance_to(test_point) << std::endl;
    std::cout << "测试点在plane1上的投影 = " << plane1.project(test_point) << std::endl;
    std::cout << "测试点关于plane1的对称点 = " << plane1.reflect(test_point) << std::endl;
    
    // 直线与平面的关系
    Line line(Point(1.0f, 1.0f, -1.0f), Point(1.0f, 1.0f, 3.0f));
    std::cout << "测试线段 = " << line << std::endl;
    std::cout << "测试线段与plane1是否相交: " << (plane1.intersects(line) ? "是" : "否") << std::endl;
    
    auto intersection = plane1.intersection_with(line);
    if (intersection) {
        std::cout << "测试线段与plane1的交点 = " << *intersection << std::endl;
    } else {
        std::cout << "测试线段与plane1不相交" << std::endl;
    }
    
    // 平面之间的关系
    std::cout << "plane1和plane2的夹角 = " << plane1.angle_with(plane2) << " 弧度" << std::endl;
    std::cout << "plane1和plane2的夹角 = " << geometry::utils::radians_to_degrees(plane1.angle_with(plane2)) << " 度" << std::endl;
    std::cout << "plane1和plane2是否平行: " << (plane1.is_parallel_to(plane2) ? "是" : "否") << std::endl;
}

// 演示多边形操作
void demo_polygons() {
    print_separator("多边形操作演示");
    
    // 创建多边形
    Polygon square;
    square.add_vertex(Point(0.0f, 0.0f, 0.0f));
    square.add_vertex(Point(2.0f, 0.0f, 0.0f));
    square.add_vertex(Point(2.0f, 2.0f, 0.0f));
    square.add_vertex(Point(0.0f, 2.0f, 0.0f));
    
    std::cout << "正方形 = " << square << std::endl;
    
    // 多边形属性
    std::cout << "正方形的面积 = " << square.area() << std::endl;
    std::cout << "正方形的周长 = " << square.perimeter() << std::endl;
    std::cout << "正方形的重心 = " << square.centroid() << std::endl;
    std::cout << "正方形是否为凸多边形: " << (square.is_convex() ? "是" : "否") << std::endl;
    
    // 点与多边形的关系
    Point test_point(1.0f, 1.0f, 0.0f);
    std::cout << "测试点 = " << test_point << std::endl;
    std::cout << "测试点是否在正方形内: " << (square.contains_point(test_point) ? "是" : "否") << std::endl;
    std::cout << "测试点到正方形的距离 = " << square.distance_to(test_point) << std::endl;
    
    // 凸包计算
    std::vector<Point> random_points = {
        Point(3.0f, 1.0f, 0.0f),
        Point(1.0f, 1.0f, 0.0f),
        Point(2.0f, 2.0f, 0.0f),
        Point(2.0f, 3.0f, 0.0f),
        Point(3.0f, 3.0f, 0.0f),
        Point(4.0f, 2.0f, 0.0f)
    };
    
    Polygon random_poly(random_points);
    std::cout << "随机多边形 = " << random_poly << std::endl;
    
    Polygon hull = random_poly.convex_hull();
    std::cout << "随机多边形的凸包 = " << hull << std::endl;
    
    // 边界框
    auto [min_point, max_point] = square.bounding_box();
    std::cout << "正方形的边界框: 最小点 = " << min_point << ", 最大点 = " << max_point << std::endl;
}

// 演示几何工具函数
void demo_utils() {
    print_separator("几何工具函数演示");
    
    // 点操作
    Point p1(1.0f, 2.0f, 3.0f);
    Point p2(4.0f, 5.0f, 6.0f);
    Point p3(7.0f, 8.0f, 9.0f);
    Point p4(1.0f, 0.0f, 0.0f);
    
    std::cout << "p1 = " << p1 << std::endl;
    std::cout << "p2 = " << p2 << std::endl;
    std::cout << "p3 = " << p3 << std::endl;
    std::cout << "p4 = " << p4 << std::endl;
    
    // 距离计算
    std::cout << "p1到p2的距离 = " << geometry::utils::distance(p1, p2) << std::endl;
    
    // 共线性检测
    std::cout << "p1, p2, p3是否共线: " << (geometry::utils::are_collinear(p1, p2, p3) ? "是" : "否") << std::endl;
    
    // 共面性检测
    std::cout << "p1, p2, p3, p4是否共面: " << (geometry::utils::are_coplanar(p1, p2, p3, p4) ? "是" : "否") << std::endl;
    
    // 三角形面积
    std::cout << "p1, p2, p3组成的三角形面积 = " << geometry::utils::triangle_area(p1, p2, p3) << std::endl;
    
    // 四面体体积
    std::cout << "p1, p2, p3, p4组成的四面体体积 = " << geometry::utils::tetrahedron_volume(p1, p2, p3, p4) << std::endl;
    
    // 向量夹角
    Point v1(1.0f, 0.0f, 0.0f);
    Point v2(0.0f, 1.0f, 0.0f);
    std::cout << "v1 = " << v1 << ", v2 = " << v2 << std::endl;
    std::cout << "v1和v2的夹角 = " << geometry::utils::angle_between(v1, v2) << " 弧度" << std::endl;
    std::cout << "v1和v2的夹角 = " << geometry::utils::radians_to_degrees(geometry::utils::angle_between(v1, v2)) << " 度" << std::endl;
    
    // 平面交线
    Plane plane1(Point(0.0f, 0.0f, 1.0f), Point(0.0f, 0.0f, 0.0f));
    Plane plane2(Point(0.0f, 1.0f, 0.0f), Point(0.0f, 0.0f, 0.0f));
    
    std::cout << "plane1 = " << plane1 << std::endl;
    std::cout << "plane2 = " << plane2 << std::endl;
    
    auto intersection_line = geometry::utils::intersection(plane1, plane2);
    if (intersection_line) {
        std::cout << "plane1和plane2的交线 = " << *intersection_line << std::endl;
    } else {
        std::cout << "plane1和plane2不相交" << std::endl;
    }
    
    // 三平面交点
    Plane plane3(Point(1.0f, 0.0f, 0.0f), Point(0.0f, 0.0f, 0.0f));
    std::cout << "plane3 = " << plane3 << std::endl;
    
    auto intersection_point = geometry::utils::intersection(plane1, plane2, plane3);
    if (intersection_point) {
        std::cout << "三个平面的交点 = " << *intersection_point << std::endl;
    } else {
        std::cout << "三个平面不存在唯一交点" << std::endl;
    }
}

int main() {
    std::cout << "C++ 几何工具库演示程序" << std::endl;
    
    // 设置输出精度
    std::cout << std::fixed << std::setprecision(2);
    
    // 演示各种几何操作
    demo_points();
    demo_lines();
    demo_planes();
    demo_polygons();
    demo_utils();
    
    print_separator("演示结束");
    return 0;
}
