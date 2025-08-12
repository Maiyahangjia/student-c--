#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <mysql/mysql.h>
#include <string>
#include <memory>
#include <stdexcept>

// 数据库连接类
class DatabaseConnection {
private:
    MYSQL* mysql; // MySQL连接句柄
    std::string host; // 数据库主机地址
    std::string user; // 数据库用户名
    std::string password; // 数据库密码
    std::string database; // 数据库名称
    unsigned int port; // 数据库端口

public:
    // 构造函数
    DatabaseConnection(const std::string& host, const std::string& user, 
                      const std::string& password, const std::string& database, 
                      unsigned int port = 3306)
        : host(host), user(user), password(password), database(database), port(port), mysql(nullptr) {
        // 初始化MySQL句柄
        mysql = mysql_init(nullptr);
        if (!mysql) {
            throw std::runtime_error("Failed to initialize MySQL connection object");
        }
    }

    // 析构函数
    ~DatabaseConnection() {
        if (mysql) {
            mysql_close(mysql);
            mysql = nullptr;
        }
    }

    // 禁止拷贝构造函数和拷贝赋值运算符
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

    // 移动构造函数和移动赋值运算符
    DatabaseConnection(DatabaseConnection&& other) noexcept
        : mysql(other.mysql), host(std::move(other.host)), user(std::move(other.user)),
          password(std::move(other.password)), database(std::move(other.database)), port(other.port) {
        other.mysql = nullptr;
    }

    DatabaseConnection& operator=(DatabaseConnection&& other) noexcept {
        if (this != &other) {
            if (mysql) {
                mysql_close(mysql);
            }
            mysql = other.mysql;
            host = std::move(other.host);
            user = std::move(other.user);
            password = std::move(other.password);
            database = std::move(other.database);
            port = other.port;
            other.mysql = nullptr;
        }
        return *this;
    }

    // 连接数据库
    bool connect() {
        if (!mysql_real_connect(mysql, host.c_str(), user.c_str(), password.c_str(),
                                database.c_str(), port, nullptr, 0)) {
            return false;
        }
        // 设置字符集
        mysql_set_character_set(mysql, "utf8mb4");
        return true;
    }

    // 执行SQL查询（不返回结果）
    bool executeQuery(const std::string& query) {
        if (mysql_query(mysql, query.c_str()) != 0) {
            return false;
        }
        return true;
    }

    // 执行SQL查询（返回结果集）
    MYSQL_RES* executeSelectQuery(const std::string& query) {
        if (mysql_query(mysql, query.c_str()) != 0) {
            return nullptr;
        }
        return mysql_store_result(mysql);
    }

    // 获取最后一次错误信息
    std::string getLastError() {
        return mysql_error(mysql);
    }
};

#endif // DATABASE_CONNECTION_H