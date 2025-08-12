#include "DatabaseConnection.h"
#include "Student.h"
#include "StudentManager.h"
#include "RandomStudentGenerator.h"
#include <iostream>
#include <string>
#include <memory>

// 生成11个随机学生并添加到数据库
void generateRandomStudents(StudentManager& manager) {
    RandomStudentGenerator generator;
    std::vector<Student> students = generator.generate11Students();

    int successCount = 0;
    for (const auto& student : students) {
        if (manager.addStudent(student)) {
            successCount++;
        }
    }

    std::cout << "成功添加了 " << successCount << " 个随机学生!" << std::endl;
}

// 显示菜单
void showMenu() {
    std::cout << "==================== 学生管理系统 ====================" << std::endl;
    std::cout << "1. 添加学生" << std::endl;
    std::cout << "2. 更新学生信息" << std::endl;
    std::cout << "3. 删除学生" << std::endl;
    std::cout << "4. 查询单个学生" << std::endl;
    std::cout << "5. 查询所有学生" << std::endl;
    std::cout << "6. 初始化数据库表" << std::endl;
    std::cout << "7. 生成11个随机学生" << std::endl;
    std::cout << "0. 退出系统" << std::endl;
    std::cout << "请选择操作: " << std::endl;
}

// 添加学生
void addStudent(StudentManager& manager) {
    std::string name, gender, studentId, major, contact;
    int age;

    std::cout << "请输入学生姓名: " << std::endl;
    std::cin >> name;
    std::cout << "请输入学生年龄: " << std::endl;
    std::cin >> age;
    std::cout << "请输入学生性别: " << std::endl;
    std::cin >> gender;
    std::cout << "请输入学生学号: " << std::endl;
    std::cin >> studentId;
    std::cout << "请输入学生专业: " << std::endl;
    std::cin >> major;
    std::cout << "请输入学生联系方式 (可选): " << std::endl;
    std::cin.ignore(); // 忽略前一个输入留下的换行符
    std::getline(std::cin, contact);

    Student student(name, age, gender, studentId, major, contact);
    if (manager.addStudent(student)) {
        std::cout << "学生添加成功!" << std::endl;
    } else {
        std::cout << "学生添加失败!" << std::endl;
    }
}

// 更新学生信息
void updateStudent(StudentManager& manager) {
    int id;
    std::cout << "请输入要更新的学生ID: " << std::endl;
    std::cin >> id;

    Student student = manager.getStudentById(id);
    if (student.getId() == 0) {
        std::cout << "未找到该学生!" << std::endl;
        return;
    }

    std::cout << "当前学生信息: " << std::endl;
    student.print();

    std::string name, gender, studentId, major, contact;
    int age;

    std::cout << "请输入新的学生姓名: " << std::endl;
    std::cin >> name;
    std::cout << "请输入新的学生年龄: " << std::endl;
    std::cin >> age;
    std::cout << "请输入新的学生性别: " << std::endl;
    std::cin >> gender;
    std::cout << "请输入新的学生学号: " << std::endl;
    std::cin >> studentId;
    std::cout << "请输入新的学生专业: " << std::endl;
    std::cin >> major;
    std::cout << "请输入新的学生联系方式 (可选): " << std::endl;
    std::cin.ignore(); // 忽略前一个输入留下的换行符
    std::getline(std::cin, contact);

    student.setName(name);
    student.setAge(age);
    student.setGender(gender);
    student.setStudentId(studentId);
    student.setMajor(major);
    student.setContact(contact);

    if (manager.updateStudent(student)) {
        std::cout << "学生信息更新成功!" << std::endl;
    } else {
        std::cout << "学生信息更新失败!" << std::endl;
    }
}

// 删除学生
void deleteStudent(StudentManager& manager) {
    int id;
    std::cout << "请输入要删除的学生ID: " << std::endl;
    std::cin >> id;

    if (manager.deleteStudent(id)) {
        std::cout << "学生删除成功!" << std::endl;
    } else {
        std::cout << "学生删除失败!" << std::endl;
    }
}

// 查询单个学生
void queryStudent(StudentManager& manager) {
    int id;
    std::cout << "请输入要查询的学生ID: " << std::endl;
    std::cin >> id;

    Student student = manager.getStudentById(id);
    if (student.getId() == 0) {
        std::cout << "未找到该学生!" << std::endl;
    } else {
        std::cout << "学生信息: " << std::endl;
        student.print();
    }
}

// 查询所有学生
void queryAllStudents(StudentManager& manager) {
    std::vector<Student> students = manager.getAllStudents();
    if (students.empty()) {
        std::cout << "暂无学生记录!" << std::endl;
    } else {
        std::cout << "所有学生信息: " << std::endl;
        for (const auto& student : students) {
            student.print();
        }
    }
}

int main() {
    // 数据库连接信息
    std::string host = "101.132.190.127";
    std::string user = "root"; // 默认用户名，实际使用时需要修改
    std::string password = "password"; // 默认密码，实际使用时需要修改
    std::string database = "student_db"; // 数据库名称
    unsigned int port = 3306; // 数据库端口

    try {
        // 创建数据库连接
        auto dbConnection = std::make_unique<DatabaseConnection>(host, user, password, database, port);

        // 连接数据库
        if (!dbConnection->connect()) {
            std::cerr << "数据库连接失败: " << dbConnection->getLastError() << std::endl;
            return 1;
        }

        std::cout << "数据库连接成功!" << std::endl;

        // 创建学生管理器
        StudentManager manager(std::move(dbConnection));

        int choice;
        while (true) {
            showMenu();
            std::cin >> choice;

            switch (choice) {
                case 1:
                    addStudent(manager);
                    break;
                case 2:
                    updateStudent(manager);
                    break;
                case 3:
                    deleteStudent(manager);
                    break;
                case 4:
                    queryStudent(manager);
                    break;
                case 5:
                    queryAllStudents(manager);
                    break;
                case 6:
                    if (manager.initDatabase()) {
                        std::cout << "数据库表初始化成功!" << std::endl;
                    } else {
                        std::cout << "数据库表初始化失败!" << std::endl;
                    }
                    break;
                case 7:
                    generateRandomStudents(manager);
                    break;
                case 0:
                    std::cout << "谢谢使用，再见!" << std::endl;
                    return 0;
                default:
                    std::cout << "无效的选择，请重新输入!" << std::endl;
                    break;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "发生异常: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}