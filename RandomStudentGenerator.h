#ifndef RANDOM_STUDENT_GENERATOR_H
#define RANDOM_STUDENT_GENERATOR_H

#include "Student.h"
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include <array>

// 随机学生生成器类
class RandomStudentGenerator {
private:
    // 随机数引擎
    std::mt19937 rng;

    // 姓名列表
    std::array<std::string, 20> firstNames = {
        "张三", "李四", "王五", "赵六", "钱七",
        "孙八", "周九", "吴十", "郑一", "王二",
        "刘一", "陈二", "杨三", "黄四", "赵五",
        "吴六", "周七", "徐八", "孙九", "马十"
    };

    // 专业列表
    std::array<std::string, 10> majors = {
        "计算机科学与技术", "软件工程", "电子信息工程",
        "通信工程", "自动化", "数学与应用数学",
        "物理学", "化学", "生物学", "医学"
    };

    // 随机生成性别
    std::string generateGender() {
        std::uniform_int_distribution<int> dist(0, 1);
        return dist(rng) == 0 ? "男" : "女";
    }

    // 随机生成年龄
    int generateAge() {
        std::uniform_int_distribution<int> dist(18, 25);
        return dist(rng);
    }

    // 随机生成学号
    std::string generateStudentId() {
        std::uniform_int_distribution<int> dist(100000, 999999);
        return "2023" + std::to_string(dist(rng));
    }

    // 随机生成联系方式
    std::string generateContact() {
        std::uniform_int_distribution<int> dist(1000000000, 9999999999);
        return "13" + std::to_string(dist(rng));
    }

public:
    // 构造函数，使用当前时间作为随机种子
    RandomStudentGenerator() {
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        rng.seed(static_cast<unsigned int>(seed));
    }

    // 生成随机学生信息
    Student generateStudent() {
        std::uniform_int_distribution<int> nameDist(0, firstNames.size() - 1);
        std::uniform_int_distribution<int> majorDist(0, majors.size() - 1);

        std::string name = firstNames[nameDist(rng)];
        int age = generateAge();
        std::string gender = generateGender();
        std::string studentId = generateStudentId();
        std::string major = majors[majorDist(rng)];
        std::string contact = generateContact();

        return Student(name, age, gender, studentId, major, contact);
    }

    // 生成11个随机学生信息
    std::vector<Student> generate11Students() {
        std::vector<Student> students;
        students.reserve(11);

        for (int i = 0; i < 11; ++i) {
            students.push_back(generateStudent());
        }

        return students;
    }
};

#endif // RANDOM_STUDENT_GENERATOR_H