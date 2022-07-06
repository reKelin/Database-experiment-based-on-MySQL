-- 事务1:
use testdb1;
start transaction;
set @n = sleep(5);
select tickets from ticket where flight_no = 'MU2455';
select tickets from ticket where flight_no = 'MU2455';
commit;
