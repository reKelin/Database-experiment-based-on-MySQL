select
    t3.t as pro_purchase_time,
    t3.amount as total_amount
from (
    select 
        *,
        count(*) over(partition by t2.workday - t2.rownum) cnt
    from (
        select
            *,
            row_number() over(order by workday) rownum
        from (
            select
                pro_purchase_time t,
                sum(pro_quantity * f_amount) amount,
                @row := datediff(pro_purchase_time, "2021-12-31") - 2 * week(pro_purchase_time) workday
            from property, fund, (select @row) a
            where pro_purchase_time like "2022-02-%"
            and pro_type = 3
            and pro_pif_id = f_id
            group by pro_purchase_time
        ) t1
        where amount >= 100000
    ) t2
) t3
where t3.cnt >= 3;