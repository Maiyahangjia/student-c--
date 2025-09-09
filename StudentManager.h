#ifndef STUDENT_MANAGER_H
#define STUDENT_MANAGER_H

#include "DatabaseConnection.h"
#include "Student.h"
#include "Logger.h"
#include <vector>
#include <memory>

// 学生管理类
class StudentManager {
private:
    std::unique_ptr<DatabaseConnection> dbConnection; // 数据库连接

public:
    // 构造函数
    StudentManager(std::unique_ptr<DatabaseConnection> dbConn)
        : dbConnection(std::move(dbConn)) {
        LOG_INFO("StudentManager initialized with database connection");
    }

    // 添加学生
    bool addStudent(const Student& student) {
        LOG_INFO("Attempting to add student: " + student.getName() + ", student ID: " + student.getStudentId());
        
        std::string query = "INSERT INTO students (name, age, gender, student_id, major, contact) VALUES (\"";
        query += student.getName() + "\", " + std::to_string(student.getAge()) + ", \"";
        query += student.getGender() + "\", \"";
        query += student.getStudentId() + "\", \"";
        query += student.getMajor() + "\", \"";
        query += student.getContact() + "\")";

        if (!dbConnection->executeQuery(query)) {
            LOG_ERROR("Failed to add student: " + student.getName() + ", error: " + dbConnection->getLastError());
            return false;
        }
        
        LOG_INFO("Successfully added student: " + student.getName() + ", student ID: " + student.getStudentId());
        return true;
    }

    // 更新学生信息
    bool updateStudent(const Student& student) {
        LOG_INFO("Attempting to update student ID: " + std::to_string(student.getId()) + ", name: " + student.getName());
        
        std::string query = "UPDATE students SET name = \"";
        query += student.getName() + "\", age = " + std::to_string(student.getAge()) + ", gender = \"";
        query += student.getGender() + "\", student_id = \"";
        query += student.getStudentId() + "\", major = \"";
        query += student.getMajor() + "\", contact = \"";
        query += student.getContact() + "\" WHERE id = " + std::to_string(student.getId());

        if (!dbConnection->executeQuery(query)) {
            LOG_ERROR("Failed to update student ID: " + std::to_string(student.getId()) + ", error: " + dbConnection->getLastError());
            return false;
        }
        
        LOG_INFO("Successfully updated student ID: " + std::to_string(student.getId()) + ", name: " + student.getName());
        return true;
    }

    // 删除学生
    bool deleteStudent(int studentId) {
        LOG_INFO("Attempting to delete student ID: " + std::to_string(studentId));
        
        std::string query = "DELETE FROM students WHERE id = " + std::to_string(studentId);

        if (!dbConnection->executeQuery(query)) {
            LOG_ERROR("Failed to delete student ID: " + std::to_string(studentId) + ", error: " + dbConnection->getLastError());
            return false;
        }
        
        LOG_INFO("Successfully deleted student ID: " + std::to_string(studentId));
        return true;
    }

    // 根据ID查询学生
    Student getStudentById(int studentId) {
        LOG_INFO("Attempting to fetch student by ID: " + std::to_string(studentId));
        
        std::string query = "SELECT * FROM students WHERE id = " + std::to_string(studentId);
        MYSQL_RES* result = dbConnection->executeSelectQuery(query);
        Student student;

        if (result) {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row) {
                student.setId(std::stoi(row[0]));
                student.setName(row[1]);
                student.setAge(std::stoi(row[2]));
                student.setGender(row[3]);
                student.setStudentId(row[4]);
                student.setMajor(row[5]);
                student.setContact(row[6] ? row[6] : "");
                LOG_INFO("Successfully fetched student ID: " + std::to_string(studentId) + ", name: " + student.getName());
            } else {
                LOG_WARNING("Student not found by ID: " + std::to_string(studentId));
            }
            mysql_free_result(result);
        } else {
            LOG_ERROR("Failed to execute query for student ID: " + std::to_string(studentId) + ", error: " + dbConnection->getLastError());
        }

        return student;
    }

    // 查询所有学生
    std::vector<Student> getAllStudents() {
        LOG_INFO("Attempting to fetch all students");
        
        std::vector<Student> students;
        std::string query = "SELECT * FROM students";
        MYSQL_RES* result = dbConnection->executeSelectQuery(query);

        if (result) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(result))) {
                Student student;
                student.setId(std::stoi(row[0]));
                student.setName(row[1]);
                student.setAge(std::stoi(row[2]));
                student.setGender(row[3]);
                student.setStudentId(row[4]);
                student.setMajor(row[5]);
                student.setContact(row[6] ? row[6] : "");
                students.push_back(student);
            }
            mysql_free_result(result);
            LOG_INFO("Successfully fetched " + std::to_string(students.size()) + " students");
        } else {
            LOG_ERROR("Failed to execute query for all students, error: " + dbConnection->getLastError());
        }

        return students;
    }

    // 初始化数据库表
    bool initDatabase() {
        LOG_INFO("Attempting to initialize database table: students");
        
        // 读取并执行create_table.sql文件中的SQL语句
        // 这里简化处理，直接执行创建表的语句
        std::string query = "CREATE TABLE IF NOT EXISTS students ("
                           "id INT PRIMARY KEY AUTO_INCREMENT,"
                           "name VARCHAR(100) NOT NULL,"
                           "age INT NOT NULL,"
                           "gender VARCHAR(20) NOT NULL,"
                           "student_id VARCHAR(20) NOT NULL UNIQUE,"
                           "major VARCHAR(100) NOT NULL,"
                           "contact VARCHAR(100)"
                           ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;";

        if (!dbConnection->executeQuery(query)) {
            LOG_ERROR("Failed to initialize database table: " + dbConnection->getLastError());
            return false;
        }
        
        LOG_INFO("Successfully initialized database table: students");
        return true;
    }
};

#endif // STUDENT_MANAGER_H