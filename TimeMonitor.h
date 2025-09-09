#ifndef TIME_MONITOR_H
#define TIME_MONITOR_H

#include <chrono>
#include <string>
#include <iostream>
#include <memory>
#include "Logger.h"
#include "DatabaseConnection.h"

// 计时器类，用于监控代码执行耗时
class TimeMonitor {
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    
    TimePoint startTime;
    TimePoint endTime;
    bool isRunning;
    std::string operationName;
    std::shared_ptr<DatabaseConnection> dbConnection; // 数据库连接指针

public:
    // 构造函数
    TimeMonitor() : isRunning(false), dbConnection(nullptr) {}
    
    // 带操作名称的构造函数
    explicit TimeMonitor(const std::string& name) : isRunning(false), operationName(name), dbConnection(nullptr) {}
    
    // 开始计时
    void start() {
        startTime = Clock::now();
        isRunning = true;
    }
    
    // 开始计时，并设置操作名称
    void start(const std::string& name) {
        operationName = name;
        start();
    }
    
    // 结束计时并返回耗时（毫秒）
    double stop() {
        if (!isRunning) {
            return 0.0;
        }
        
        endTime = Clock::now();
        isRunning = false;
        
        std::chrono::duration<double, std::milli> duration = endTime - startTime;
        double elapsedTime = duration.count();
        
        // 输出耗时信息
        if (!operationName.empty()) {
            std::string message = operationName + " 执行耗时: " + std::to_string(elapsedTime) + " ms";
            std::cout << message << std::endl;
            LOG_INFO(message);
        }
        
        // 如果设置了数据库连接，则保存到数据库
        if (dbConnection) {
            saveToDatabase(elapsedTime);
        }
        
        return elapsedTime;
    }
    
    // 设置数据库连接
    void setDatabaseConnection(std::shared_ptr<DatabaseConnection> dbConn) {
        dbConnection = dbConn;
        LOG_INFO("Database connection set for TimeMonitor");
    }
    
    // 获取操作名称
    std::string getOperationName() const {
        return operationName;
    }
    
    // 设置操作名称
    void setOperationName(const std::string& name) {
        operationName = name;
    }
        
private:
    // 将操作耗时信息保存到数据库
    void saveToDatabase(double durationMs) {
        try {
            // 格式化时间戳
            auto start_time_t = std::chrono::system_clock::to_time_t(startTime);
            auto end_time_t = std::chrono::system_clock::to_time_t(endTime);
            
            // 转换为MySQL DATETIME格式
            char startTimeStr[20];
            char endTimeStr[20];
            strftime(startTimeStr, sizeof(startTimeStr), "%Y-%m-%d %H:%M:%S", localtime(&start_time_t));
            strftime(endTimeStr, sizeof(endTimeStr), "%Y-%m-%d %H:%M:%S", localtime(&end_time_t));
            
            // 构建SQL查询
            std::string query = "INSERT INTO operation_logs (operation_name, start_time, end_time, duration_ms) VALUES ('";
            query += escapeString(operationName) + "', '" + startTimeStr + "', '" + endTimeStr + "', " + std::to_string(durationMs) + ")";
            
            // 执行查询
            if (!dbConnection->executeQuery(query)) {
                LOG_ERROR("Failed to save operation log to database: " + dbConnection->getLastError());
            } else {
                LOG_INFO("Successfully saved operation log to database");
            }
        } catch (const std::exception& e) {
            LOG_ERROR("Exception when saving operation log: " + std::string(e.what()));
        }
    }
    
    // 字符串转义，防止SQL注入
    std::string escapeString(const std::string& str) {
        std::string escaped;
        for (char c : str) {
            if (c == '"' || c == '\\' || c == '\'') {
                escaped += '\\';
            }
            escaped += c;
        }
        return escaped;
    }
    
    // 检查是否正在计时
    bool running() const {
        return isRunning;
    }
};

#endif // TIME_MONITOR_H