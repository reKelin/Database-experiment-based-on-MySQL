select
    wk week_of_trading,
    sum(case dayId when 0 then amount end) Monday,
    sum(case dayId when 1 then amount end) Tuesday,
    sum(case dayId when 2 then amount end) Wednesday,
    sum(case dayId when 3 then amount end) Thursday,
    sum(case dayId when 4 then amount end) Friday
from (
    select
        week(pro_purchase_time) - 5 wk,
        weekday(pro_purchase_time) dayId,
        sum(pro_quantity * f_amount) amount
    from property
    join fund
    on pro_pif_id = f_id
    where pro_purchase_time like "2022-02-%"
    and pro_type = 3
    group by pro_purchase_time
) t
group by wk;