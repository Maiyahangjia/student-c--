#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <mysql/mysql.h>
#include <string>
#include <memory>
#include <stdexcept>
#include "Logger.h"

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
            LOG_ERROR("Failed to initialize MySQL connection object");
            throw std::runtime_error("Failed to initialize MySQL connection object");
        }
        LOG_INFO("DatabaseConnection object created for host: " + host + ", database: " + database);
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
        LOG_INFO("Attempting to connect to MySQL server at " + host + ":" + std::to_string(port));
        
        // 设置连接超时为10秒，避免程序无限等待
        mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, "10");
        
        // 尝试连接数据库
        if (!mysql_real_connect(mysql, host.c_str(), user.c_str(), password.c_str(),
                                database.c_str(), port, nullptr, 0)) {
            LOG_ERROR("Failed to connect to MySQL server: " + getLastError());
            return false;
        }
        // 设置字符集
        mysql_set_character_set(mysql, "utf8mb4");
        LOG_INFO("Successfully connected to MySQL server: " + host + ", database: " + database);
        return true;
    }

    // 执行SQL查询（不返回结果）
    bool executeQuery(const std::string& query) {
        // 仅记录查询类型，不记录完整查询内容（避免日志过大）
        std::string queryType = query.substr(0, query.find_first_of(" "));
        for (char &c : queryType) c = toupper(c);
        LOG_INFO("Executing SQL query: " + queryType);
        
        if (mysql_query(mysql, query.c_str()) != 0) {
            LOG_ERROR("Failed to execute SQL query: " + getLastError());
            return false;
        }
        return true;
    }

    // 执行SQL查询（返回结果集）
    MYSQL_RES* executeSelectQuery(const std::string& query) {
        // 仅记录查询类型，不记录完整查询内容
        std::string queryType = query.substr(0, query.find_first_of(" "));
        for (char &c : queryType) c = toupper(c);
        LOG_INFO("Executing SQL select query: " + queryType);
        
        if (mysql_query(mysql, query.c_str()) != 0) {
            LOG_ERROR("Failed to execute SQL select query: " + getLastError());
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