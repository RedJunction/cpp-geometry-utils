# C++ 几何工具库 (cpp-geometry-utils)

一个现代C++17几何计算库，提供了丰富的几何对象表示和操作功能，包括点、线、平面和多边形等基本几何元素。

## 特性

- **点 (Point)**: 高效的2D和3D点表示，支持向量运算（加、减、乘、除）、点积、叉积、距离计算等。
- **线段 (Line)**: 支持线段表示和操作，包括长度计算、方向向量、中点、点到线段的距离、投影点、对称点、线段相交检测等。
- **平面 (Plane)**: 3D平面表示，支持点到平面的距离、投影、对称点计算，以及平面与直线的相交检测等。
- **多边形 (Polygon)**: 支持多边形操作，包括面积计算、周长计算、点包含测试、凸包计算、多边形简化等。
- **几何工具函数**: 提供丰富的几何计算工具，如距离计算、交点计算、共线/共面检测、三角形面积、四面体体积等。
- **贝塞尔曲线**: 支持二阶和三阶贝塞尔曲线的计算。

## 要求

- C++17兼容的编译器
- CMake 3.10或更高版本

## 构建和安装

```bash
# 克隆仓库
git clone https://github.com/yourusername/cpp-geometry-utils.git
cd cpp-geometry-utils

# 创建构建目录
mkdir build && cd build

# 配置和构建
cmake ..
make

# 运行演示程序
./geometry-utils
```

## 使用示例

### 点操作

```cpp
#include "geometry/Point.h"
#include <iostream>

int main() {
    // 创建点
    Point p1(1.0f, 2.0f, 3.0f);
    Point p2(4.0f, 5.0f, 6.0f);
    
    // 点的运算
    Point sum = p1 + p2;
    Point diff = p2 - p1;
    Point scaled = p1 * 2.5;
    
    // 点的属性
    double magnitude = p1.magnitude();
    Point unit = p1.normalized();
    double distance = p1.distance_to(p2);
    
    // 向量运算
    double dot = dot_product(p1, p2);
    Point cross = cross_product(p1, p2);
    
    std::cout << "p1 = " << p1 << std::endl;
    std::cout << "p2 = " << p2 << std::endl;
    std::cout << "p1 + p2 = " << sum << std::endl;
    std::cout << "p1的模长 = " << magnitude << std::endl;
    std::cout << "p1 · p2 = " << dot << std::endl;
    
    return 0;
}
```

### 线段操作

```cpp
#include "geometry/Line.h"
#include <iostream>

int main() {
    // 创建线段
    Point p1(0.0f, 0.0f, 0.0f);
    Point p2(3.0f, 4.0f, 0.0f);
    Line line(p1, p2);
    
    // 线段属性
    float length = line.length();
    Point direction = line.direction();
    Point midpoint = line.midpoint();
    
    // 点与线段的关系
    Point test(1.0f, 2.0f, 0.0f);
    float distance = line.distance_to(test);
    Point projection = line.project(test);
    
    std::cout << "线段长度: " << length << std::endl;
    std::cout << "点到线段的距离: " << distance << std::endl;
    
    return 0;
}
```

### 平面操作

```cpp
#include "geometry/Plane.h"
#include <iostream>

int main() {
    // 从三点创建平面
    Point p1(0.0f, 0.0f, 0.0f);
    Point p2(1.0f, 0.0f, 0.0f);
    Point p3(0.0f, 1.0f, 0.0f);
    Plane plane(p1, p2, p3);
    
    // 点与平面的关系
    Point test(1.0f, 1.0f, 1.0f);
    float distance = plane.distance_to(test);
    Point projection = plane.project(test);
    
    // 直线与平面的交点
    Line line(Point(0.0f, 0.0f, 1.0f), Point(1.0f, 1.0f, 2.0f));
    auto intersection = plane.intersection_with(line);
    
    if (intersection) {
        std::cout << "交点: " << *intersection << std::endl;
    }
    
    return 0;
}
```

### 多边形操作

```cpp
#include "geometry/Polygon.h"
#include <iostream>

int main() {
    // 创建多边形
    Polygon polygon;
    polygon.add_vertex(Point(0.0f, 0.0f, 0.0f));
    polygon.add_vertex(Point(2.0f, 0.0f, 0.0f));
    polygon.add_vertex(Point(2.0f, 2.0f, 0.0f));
    polygon.add_vertex(Point(0.0f, 2.0f, 0.0f));
    
    // 多边形属性
    float area = polygon.area();
    float perimeter = polygon.perimeter();
    Point centroid = polygon.centroid();
    
    // 点包含测试
    Point test(1.0f, 1.0f, 0.0f);
    bool contains = polygon.contains_point(test);
    
    std::cout << "多边形面积: " << area << std::endl;
    std::cout << "点是否在多边形内: " << (contains ? "是" : "否") << std::endl;
    
    return 0;
}
```

### 几何工具函数

```cpp
#include "utils/utils.h"
#include <iostream>

int main() {
    using namespace geometry::utils;
    
    // 点操作
    Point p1(1.0f, 0.0f, 0.0f);
    Point p2(0.0f, 1.0f, 0.0f);
    Point p3(0.0f, 0.0f, 1.0f);
    
    // 距离计算
    float dist = distance(p1, p2);
    
    // 三角形面积
    float area = triangle_area(p1, p2, p3);
    
    // 向量夹角
    float angle = angle_between(p1, p2);
    float degrees = radians_to_degrees(angle);
    
    std::cout << "p1和p2的距离: " << dist << std::endl;
    std::cout << "三角形面积: " << area << std::endl;
    std::cout << "向量夹角: " << degrees << " 度" << std::endl;
    
    return 0;
}
```

## 许可证

本项目采用MIT许可证。详情请参阅[LICENSE](LICENSE)文件。