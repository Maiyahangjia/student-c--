#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <iostream>

// 学生类
class Student {
private:
    int id; // 数据库中的ID
    std::string name; // 姓名
    int age; // 年龄
    std::string gender; // 性别
    std::string studentId; // 学号
    std::string major; // 专业
    std::string contact; // 联系方式

public:
    // 默认构造函数
    Student() : id(0), age(0) {}

    // 带参数的构造函数
    Student(const std::string& name, int age, const std::string& gender, 
           const std::string& studentId, const std::string& major, 
           const std::string& contact = "")
        : id(0), name(name), age(age), gender(gender), studentId(studentId), 
          major(major), contact(contact) {}

    // 设置ID
    void setId(int id) { this->id = id; }

    // 获取ID
    int getId() const { return id; }

    // 设置姓名
    void setName(const std::string& name) { this->name = name; }

    // 获取姓名
    std::string getName() const { return name; }

    // 设置年龄
    void setAge(int age) { this->age = age; }

    // 获取年龄
    int getAge() const { return age; }

    // 设置性别
    void setGender(const std::string& gender) { this->gender = gender; }

    // 获取性别
    std::string getGender() const { return gender; }

    // 设置学号
    void setStudentId(const std::string& studentId) { this->studentId = studentId; }

    // 获取学号
    std::string getStudentId() const { return studentId; }

    // 设置专业
    void setMajor(const std::string& major) { this->major = major; }

    // 获取专业
    std::string getMajor() const { return major; }

    // 设置联系方式
    void setContact(const std::string& contact) { this->contact = contact; }

    // 获取联系方式
    std::string getContact() const { return contact; }

    // 打印学生信息
    void print() const {
        std::cout << "ID: " << id << std::endl;
        std::cout << "姓名: " << name << std::endl;
        std::cout << "年龄: " << age << std::endl;
        std::cout << "性别: " << gender << std::endl;
        std::cout << "学号: " << studentId << std::endl;
        std::cout << "专业: " << major << std::endl;
        std::cout << "联系方式: " << contact << std::endl;
        std::cout << "------------------------" << std::endl;
    }
};

#endif // STUDENT_H