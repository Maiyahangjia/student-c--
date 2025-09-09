# C++学生管理系统

这是一个使用C++11新特性开发的学生管理系统，能够连接到远程MySQL数据库并进行学生信息的增删改查操作。

## 系统功能

1. 添加学生
2. 更新学生信息
3. 删除学生
4. 查询单个学生
5. 查询所有学生
6. 初始化数据库表
7. 生成500个随机学生信息
8. 日志打印功能：包含INFO/WARNING/ERROR三种级别，记录系统运行状态、用户操作和错误信息，支持控制台输出和文件输出，带时间戳和线程ID
9. 计时监控模块：监控每个菜单选项执行的耗时，以毫秒为单位显示并记录到日志中

## 项目结构

```
student-c++/
├── create_table.sql    # 数据库表创建语句
├── DatabaseConnection.h # 数据库连接类
├── Student.h           # 学生类
├── StudentManager.h    # 学生管理类
├── main.cpp            # 主程序文件
└── README.md           # 项目说明
```

## 环境要求

1. C++11或更高版本的编译器
2. MySQL开发库 (libmysql)
3. MySQL服务器 (远程地址: 101.132.190.127)

## 编译说明

### 使用CMake编译 (推荐)

1. 确保已安装CMake (3.10或更高版本)
2. 确保已安装MySQL开发库

3. 编译步骤:
```bash
# 创建构建目录
mkdir build
cd build

# 运行CMake生成Makefile或Visual Studio项目
cmake ..

# 编译项目
cmake --build .
```

### Windows系统

1. 安装MySQL开发库
2. 可以使用CMake生成Visual Studio项目，然后在Visual Studio中编译
3. 或者使用MinGW编译:
```
g++ -std=c++11 main.cpp -o student_manager -I"C:\Program Files\MySQL\MySQL Server 8.0\include" -L"C:\Program Files\MySQL\MySQL Server 8.0\lib" -lmysql
```

### Linux系统

1. 安装MySQL开发库
```
sudo apt-get install libmysqlclient-dev
```

2. 编译程序
```
g++ -std=c++11 main.cpp -o student_manager -I/usr/include/mysql -lmysqlclient
```

### CMake配置选项

如果CMake无法自动找到MySQL库，可以通过以下选项手动指定:
```bash
cmake .. -DMySQL_INCLUDE_DIRS="/path/to/mysql/include" -DMySQL_LIBRARIES="/path/to/mysql/lib/libmysql.lib"
```

## 使用说明

1. 配置数据库连接信息
   在main.cpp文件中修改以下代码，设置正确的数据库用户名和密码:
   ```cpp
   std::string user = "root"; // 默认用户名，实际使用时需要修改
   std::string password = "password"; // 默认密码，实际使用时需要修改
   std::string database = "student_db"; // 数据库名称
   ```

2. 运行程序
   ```
   ./student_manager
   ```

3. 初始化数据库表
   运行程序后，选择选项6初始化数据库表。

4. 使用菜单进行学生管理操作
   根据菜单提示，输入相应的数字选择要执行的操作。

5. 生成随机学生信息
   选择选项7可以自动生成11个随机学生信息并添加到数据库中。
   该功能使用C++11的随机数生成器，生成的信息包括：
   - 随机姓名
   - 随机年龄(18-25岁)
   - 随机性别
   - 随机学号
   - 随机专业
   - 随机联系方式

6. 日志功能
   系统内置了日志功能，会自动记录以下信息：
   - 系统启动和关闭日志
   - 数据库连接状态
   - 用户操作记录（添加、更新、删除、查询学生）
   - 错误信息和异常
   
   日志会同时输出到控制台和日志文件（student_manager.log），包含时间戳和线程ID，便于问题排查。
   日志分为三个级别：
   - INFO：一般信息，记录正常操作流程
   - WARNING：警告信息，表示可能的问题但不影响系统运行
   - ERROR：错误信息，记录系统异常和操作失败

## 注意事项

1. 确保远程MySQL服务器允许远程连接
2. 确保数据库用户具有足够的权限
3. 如果连接失败，请检查网络连接和MySQL服务器配置
4. 在生产环境中，请使用安全的数据库连接方式和密码存储方式

## 建表语句

系统会自动创建以下表结构:
```sql
CREATE TABLE IF NOT EXISTS students (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    age INT NOT NULL,
    gender VARCHAR(20) NOT NULL,
    student_id VARCHAR(20) NOT NULL UNIQUE,
    major VARCHAR(100) NOT NULL,
    contact VARCHAR(100)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
```