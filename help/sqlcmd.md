## Sqlite3
### Shell cmd
```
sqlite3 vchat.db # create database
vchat.db .dump > vchat.sql # export data
vchat.db < vchat.sql # import data
```
### Database cmd
```
-- set database style
.header on
.mode column
.timer on
.width 8
.exit -- exit
.show -- show setting
.table -- show all table
```
### SQL cmd
```
-- create table
CREATE TABLE [tablename]{
  [column_name] [type] PRIMARY KEY,
  [column_name] [type]
};
-- delete table
DROP TABLE [tablename];
-- insert data
INSERT INTO [tablename] VALUES ([value], [value], [value]);
-- find data
SELECT [column_name] FROM [tablename];
-- delete data
DELETE FROM [tablename] WHERE [condition];
```
