select
    c_name,
    c_phone,
    c_mail
from client
where c_id in (
    select
        pro_c_id
    from property 
    where pro_type = 3
    and pro_pif_id in (
        select
            f_id
        from fund
        where f_type = "货币型"
    )
)
order by c_id;