-- 学生表创建语句
CREATE TABLE IF NOT EXISTS students (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    age INT NOT NULL,
    gender VARCHAR(20) NOT NULL,
    student_id VARCHAR(20) NOT NULL UNIQUE,
    major VARCHAR(100) NOT NULL,
    contact VARCHAR(100)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 操作日志表创建语句，用于记录菜单操作耗时
CREATE TABLE IF NOT EXISTS operation_logs (
    id INT PRIMARY KEY AUTO_INCREMENT,
    operation_name VARCHAR(100) NOT NULL COMMENT '操作名称',
    start_time DATETIME NOT NULL COMMENT '开始时间',
    end_time DATETIME NOT NULL COMMENT '结束时间',
    duration_ms DOUBLE NOT NULL COMMENT '耗时毫秒数',
    log_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP COMMENT '日志记录时间'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT '操作日志表';
