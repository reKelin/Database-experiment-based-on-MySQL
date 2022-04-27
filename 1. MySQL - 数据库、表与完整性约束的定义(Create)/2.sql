create database TestDb;
use TestDb;
create table t_emp(
    id int primary key,
    deptId int,
    name varchar(32),
    salary float
);