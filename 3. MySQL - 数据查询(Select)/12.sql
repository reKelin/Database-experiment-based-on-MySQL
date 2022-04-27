select
    c_name,
    c_id_card,
    ifnull(sum(pro_amount), 0) as total_amount
from client
    left join (
        select
            pro_c_id,
            pro_quantity * p_amount as pro_amount
        from property, finances_product
        where pro_pif_id = p_id
        and pro_type = 1
        union all 
        select
            pro_c_id,
            pro_quantity * i_amount as pro_amount
        from property, insurance
        where pro_pif_id = i_id
        and pro_type = 2
        union all
        select
            pro_c_id,
            pro_quantity * f_amount as pro_amount
        from property, fund
        where pro_pif_id = f_id
        and pro_type = 3
    ) pro
    on pro.pro_c_id = c_id
group by c_id
order by total_amount desc;