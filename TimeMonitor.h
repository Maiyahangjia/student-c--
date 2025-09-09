#ifndef TIME_MONITOR_H
#define TIME_MONITOR_H

#include <chrono>
#include <string>
#include <iostream>
#include "Logger.h"

// 计时器类，用于监控代码执行耗时
class TimeMonitor {
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    
    TimePoint startTime;
    TimePoint endTime;
    bool isRunning;
    std::string operationName;

public:
    // 构造函数
    TimeMonitor() : isRunning(false) {}
    
    // 带操作名称的构造函数
    explicit TimeMonitor(const std::string& name) : isRunning(false), operationName(name) {}
    
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
        
        return elapsedTime;
    }
    
    // 获取操作名称
    std::string getOperationName() const {
        return operationName;
    }
    
    // 设置操作名称
    void setOperationName(const std::string& name) {
        operationName = name;
    }
    
    // 检查是否正在计时
    bool running() const {
        return isRunning;
    }
};

#endif // TIME_MONITOR_H