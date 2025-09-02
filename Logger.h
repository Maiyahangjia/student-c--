#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <mutex>
#include <thread>

// 日志级别枚举
enum class LogLevel {
    INFO,
    WARNING,
    ERROR
};

// 日志工具类
class Logger {
private:
    static Logger* instance; // 单例实例
    static std::mutex mutex; // 互斥锁，保证线程安全
    std::ofstream logFile; // 日志文件流
    LogLevel minLevel; // 最低日志级别
    bool consoleOutput; // 是否输出到控制台

    // 私有构造函数
    Logger() : minLevel(LogLevel::INFO), consoleOutput(true) {
        // 打开日志文件，使用追加模式
        std::string filename = getLogFileName();
        logFile.open(filename, std::ios::out | std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "无法打开日志文件: " << filename << std::endl;
        }
    }

    // 获取当前日期时间，用于日志文件名
    std::string getLogFileName() {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::tm local_tm = *std::localtime(&now_c);
        
        std::stringstream ss;
        ss << "student_system_log_" 
           << std::put_time(&local_tm, "%Y%m%d") 
           << ".log";
        return ss.str();
    }

    // 获取当前时间字符串，用于日志条目
    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()).count() % 1000;
        std::tm local_tm = *std::localtime(&now_c);
        
        std::stringstream ss;
        ss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S.") 
           << std::setw(3) << std::setfill('0') << now_ms;
        return ss.str();
    }

    // 获取线程ID字符串
    std::string getThreadId() {
        std::stringstream ss;
        ss << std::this_thread::get_id();
        return ss.str();
    }

    // 获取日志级别字符串
    std::string getLevelString(LogLevel level) {
        switch (level) {
            case LogLevel::INFO:
                return "[INFO]";
            case LogLevel::WARNING:
                return "[WARNING]";
            case LogLevel::ERROR:
                return "[ERROR]";
            default:
                return "[UNKNOWN]";
        }
    }

public:
    // 获取单例实例
    static Logger* getInstance() {
        if (instance == nullptr) {
            std::lock_guard<std::mutex> lock(mutex);
            if (instance == nullptr) {
                instance = new Logger();
            }
        }
        return instance;
    }

    // 设置最低日志级别
    void setMinLevel(LogLevel level) {
        minLevel = level;
    }

    // 设置是否输出到控制台
    void setConsoleOutput(bool output) {
        consoleOutput = output;
    }

    // 记录日志
    void log(LogLevel level, const std::string& message) {
        // 检查日志级别
        if (level < minLevel) {
            return;
        }

        std::lock_guard<std::mutex> lock(mutex);

        // 构建日志消息
        std::string timestamp = getCurrentTime();
        std::string threadId = getThreadId();
        std::string levelStr = getLevelString(level);
        std::string logMessage = timestamp + " " + threadId + " " + levelStr + " " + message + "\n";

        // 输出到控制台
        if (consoleOutput) {
            if (level == LogLevel::ERROR) {
                std::cerr << logMessage;
            } else {
                std::cout << logMessage;
            }
        }

        // 输出到文件
        if (logFile.is_open()) {
            logFile << logMessage;
            logFile.flush(); // 立即写入文件
        }
    }

    // 记录INFO级别日志
    void info(const std::string& message) {
        log(LogLevel::INFO, message);
    }

    // 记录WARNING级别日志
    void warning(const std::string& message) {
        log(LogLevel::WARNING, message);
    }

    // 记录ERROR级别日志
    void error(const std::string& message) {
        log(LogLevel::ERROR, message);
    }

    // 析构函数
    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
};

// 初始化静态成员变量
Logger* Logger::instance = nullptr;
std::mutex Logger::mutex;

// 方便使用的全局日志宏
#define LOG_INFO(msg) Logger::getInstance()->info(msg)
#define LOG_WARNING(msg) Logger::getInstance()->warning(msg)
#define LOG_ERROR(msg) Logger::getInstance()->error(msg)

#endif // LOGGER_H