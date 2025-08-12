# CMake最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(StudentManagementSystem VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找MySQL库
find_package(MySQL REQUIRED)

# 如果找不到MySQL，尝试手动设置路径
if(NOT MySQL_FOUND)
    message(WARNING "MySQL库未找到，尝试使用默认安装路径")
    # Windows系统默认路径
    if(WIN32)
        set(MySQL_INCLUDE_DIRS "C:/Program Files/MySQL/MySQL Server 8.0/include")
        set(MySQL_LIBRARIES "C:/Program Files/MySQL/MySQL Server 8.0/lib/libmysql.lib")
    # Linux系统默认路径
    else()
        set(MySQL_INCLUDE_DIRS "/usr/include/mysql")
        set(MySQL_LIBRARIES "-lmysqlclient")
    endif()
endif()

# 包含MySQL头文件目录
include_directories(${MySQL_INCLUDE_DIRS})

# 添加可执行文件
add_executable(student_manager main.cpp)

# 链接MySQL库
target_link_libraries(student_manager ${MySQL_LIBRARIES})

# 复制SQL文件到构建目录
configure_file(create_table.sql ${CMAKE_CURRENT_BINARY_DIR}/create_table.sql COPYONLY)

# 输出信息
message(STATUS "项目: ${PROJECT_NAME} ${PROJECT_VERSION}")
message(STATUS "C++标准: ${CMAKE_CXX_STANDARD}")
message(STATUS "MySQL头文件: ${MySQL_INCLUDE_DIRS}")
message(STATUS "MySQL库: ${MySQL_LIBRARIES}")