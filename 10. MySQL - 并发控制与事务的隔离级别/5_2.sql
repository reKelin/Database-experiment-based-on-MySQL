-- 事务2:
use testdb1;
set session transaction isolation level read uncommitted;
start transaction;
set @n = sleep(1);
-- 在事务1的第1，2次查询之间，试图出票1张(航班MU2455)：
update ticket set tickets = tickets - 1 where flight_no = 'MU2455';
commit;