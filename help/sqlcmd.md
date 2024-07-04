### 数据库
```
sqlite3 vchat.db # 创建数据库
vchat.db .dump > vchat.sql  -- 导出数据库
vchat.db < vchat.sql -- 导入数据库
```
### 命令
```
-- 设置数据库样式
.header on
.mode column
.timer on
.width 8
.exit -- 退出数据库
.show -- 显示设置值
.table -- 显示所有表
```
### 操作
```
-- 创建表
CREATE TABLE [tablename]{
  [column_name] [type] PRIMARY KEY,
  [column_name] [type]
};
-- 删除表
DROP TABLE [tablename];
-- 插入数据
INSERT INTO [tablename] VALUES ([value], [value], [value]);
-- 查询数据
SELECT [column_name] FROM [tablename];
-- 删除数据
DELETE FROM [tablename] WHERE [condition];
```
