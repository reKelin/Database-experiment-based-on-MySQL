select
    c_id,
    c_name, 
    ifnull(sum(amount), 0) as total_property 
from client 
    left join (
        select
            pro_c_id, 
            pro_quantity * p_amount as amount 
        from property, finances_product 
        where pro_pif_id = p_id
        and pro_type = 1
        union all
        select
            pro_c_id,
            pro_quantity * i_amount as amount
        from property, insurance
        where pro_pif_id = i_id
        and pro_type = 2
        union all
        select
            pro_c_id,
            pro_quantity * f_amount as amount
        from property, fund
        where pro_pif_id = f_id
        and pro_type = 3
        union all
        select
            pro_c_id,
            sum(pro_income) as amount
        from property
        group by pro_c_id
        union all
        select
            b_c_id,
            sum(if(b_type = "储蓄卡", b_balance, -b_balance)) as amount
        from bank_card
        group by b_c_id
    ) pro
    on c_id = pro.pro_c_id
group by c_id
order by c_id;