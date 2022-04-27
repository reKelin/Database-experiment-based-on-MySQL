use fib;

-- 创建存储过程`sp_fibonacci(in m int)`，向表fibonacci插入斐波拉契数列的前m项，及其对应的斐波拉契数。fibonacci表初始值为一张空表。请保证你的存储过程可以多次运行而不出错。

drop procedure if exists sp_fibonacci;
delimiter $$
create procedure sp_fibonacci(in m int)
begin
    set m = m - 1;
    with recursive cte (id, cur, pre) as (
        select
            0, 0, 0
        union all 
        select 
            id + 1,
            if (id < 2, 1, cur + pre),
            cur 
        from cte 
        where id < m
    ) 
    select 
        id n,
        cur fibn
    from cte;
end $$

delimiter ;