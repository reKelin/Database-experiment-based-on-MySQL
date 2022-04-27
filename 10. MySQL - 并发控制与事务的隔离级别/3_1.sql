-- 事务1:
-- 请设置适当的事务隔离级别
set session transaction isolation level read committed;

-- 开启事务
start transaction;
-- 时刻1 - 事务1读航班余票:
insert into result 
select now(),1 t, tickets from ticket where flight_no = 'CZ5525';

-- 添加等待代码，确保事务2的第一次读取在事务1修改前发生
set @n = sleep(2);

-- 时刻3 - 事务1修改余票，并立即读取:
update ticket set tickets = tickets - 1 where flight_no = 'CZ5525';
insert into result
select now(),1 t, tickets from ticket where flight_no = 'CZ5525';

-- 添加代码，使事务2 的第2次读取在事务1修改之后，提交之前发生
-- set @n = sleep(1);

commit;

-- 时刻6 - 事务1在t2也提交后读取余票
-- 添加代码，确保事务1在事务2提交后读取
set @n = sleep(3);

insert into result
select now(), 1 t, tickets from ticket where flight_no = 'CZ5525';