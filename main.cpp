#include "DatabaseConnection.h"
#include "Student.h"
#include "StudentManager.h"
#include "RandomStudentGenerator.h"
#include "Logger.h"
#include <iostream>
#include <string>
#include <memory>

// 生成11个随机学生并添加到数据库
void generateRandomStudents(StudentManager& manager) {
    LOG_INFO("开始生成500个随机学生信息");
    RandomStudentGenerator generator;
    std::vector<Student> students = generator.generate11Students();

    int successCount = 0;
    for (const auto& student : students) {
        if (manager.addStudent(student)) {
            successCount++;
            LOG_INFO("成功添加随机学生: " + student.getName() + ", 学号: " + student.getStudentId());
        } else {
            LOG_WARNING("添加随机学生失败: " + student.getName() + ", 学号: " + student.getStudentId());
        }
    }

    std::cout << "成功添加了 " << successCount << " 个随机学生!" << std::endl;
    LOG_INFO("随机学生生成完成，共成功添加 " + std::to_string(successCount) + " 个学生");
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
    std::cout << "7. 生成500个随机学生" << std::endl;
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
        LOG_INFO("用户添加学生成功: " + name + ", 学号: " + studentId);
    } else {
        std::cout << "学生添加失败!" << std::endl;
        LOG_ERROR("用户添加学生失败: " + name + ", 学号: " + studentId);
    }
}

// 更新学生信息
void updateStudent(StudentManager& manager) {
    int id;
    std::cout << "请输入要更新的学生ID: " << std::endl;
    std::cin >> id;
    LOG_INFO("用户尝试更新学生ID: " + std::to_string(id));

    Student student = manager.getStudentById(id);
    if (student.getId() == 0) {
        std::cout << "未找到该学生!" << std::endl;
        LOG_WARNING("用户更新学生失败: 未找到ID为 " + std::to_string(id) + " 的学生");
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
        LOG_INFO("用户更新学生成功: " + name + ", ID: " + std::to_string(id));
    } else {
        std::cout << "学生信息更新失败!" << std::endl;
        LOG_ERROR("用户更新学生失败: ID: " + std::to_string(id));
    }
}

// 删除学生
void deleteStudent(StudentManager& manager) {
    int id;
    std::cout << "请输入要删除的学生ID: " << std::endl;
    std::cin >> id;
    LOG_INFO("用户尝试删除学生ID: " + std::to_string(id));

    if (manager.deleteStudent(id)) {
        std::cout << "学生删除成功!" << std::endl;
        LOG_INFO("用户删除学生成功: ID: " + std::to_string(id));
    } else {
        std::cout << "学生删除失败!" << std::endl;
        LOG_ERROR("用户删除学生失败: ID: " + std::to_string(id));
    }
}

// 查询单个学生
void queryStudent(StudentManager& manager) {
    int id;
    std::cout << "请输入要查询的学生ID: " << std::endl;
    std::cin >> id;
    LOG_INFO("用户查询学生ID: " + std::to_string(id));

    Student student = manager.getStudentById(id);
    if (student.getId() == 0) {
        std::cout << "未找到该学生!" << std::endl;
        LOG_WARNING("用户查询学生未找到: ID: " + std::to_string(id));
    } else {
        std::cout << "学生信息: " << std::endl;
        student.print();
        LOG_INFO("用户查询学生成功: " + student.getName() + ", ID: " + std::to_string(id));
    }
}

// 查询所有学生
void queryAllStudents(StudentManager& manager) {
    LOG_INFO("用户查询所有学生信息");
    std::vector<Student> students = manager.getAllStudents();
    if (students.empty()) {
        std::cout << "暂无学生记录!" << std::endl;
        LOG_INFO("当前系统中暂无学生记录");
    } else {
        std::cout << "所有学生信息: " << std::endl;
        for (const auto& student : students) {
            student.print();
        }
        LOG_INFO("成功查询到 " + std::to_string(students.size()) + " 条学生记录");
    }
}

int main() {
    LOG_INFO("学生管理系统启动");
    // 数据库连接信息
    //std::string host = "101.132.190.127";
    std::string host = "localhost";
    std::string user = "zyc"; // 默认用户名，实际使用时需要修改
    std::string password = "zyc"; // 默认密码，实际使用时需要修改
    std::string database = "player"; // 数据库名称
    unsigned int port = 3306; // 数据库端口

    try {
        LOG_INFO("尝试连接数据库: " + host + ":" + std::to_string(port) + ", 数据库: " + database + ", 用户: " + user);
        // 创建数据库连接
        auto dbConnection = std::make_unique<DatabaseConnection>(host, user, password, database, port);

        // 连接数据库
        if (!dbConnection->connect()) {
            std::cerr << "数据库连接失败: " << dbConnection->getLastError() << std::endl;
            LOG_ERROR("数据库连接失败: " + dbConnection->getLastError());
            return 1;
        }

        std::cout << "数据库连接成功!" << std::endl;
        LOG_INFO("数据库连接成功");

        // 创建学生管理器
        StudentManager manager(std::move(dbConnection));
        LOG_INFO("学生管理器初始化成功");

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
                    LOG_INFO("用户执行数据库表初始化");
                    if (manager.initDatabase()) {
                        std::cout << "数据库表初始化成功!" << std::endl;
                        LOG_INFO("数据库表初始化成功");
                    } else {
                        std::cout << "数据库表初始化失败!" << std::endl;
                        LOG_ERROR("数据库表初始化失败");
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
        LOG_ERROR("系统发生异常: " + std::string(e.what()));
        return 1;
    }

    LOG_INFO("学生管理系统正常退出");
    return 0;
}