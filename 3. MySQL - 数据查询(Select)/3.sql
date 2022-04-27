select
    c_name,
    c_mail,
    c_phone
from client
where exists (
    select
        *
    from property
    where pro_c_id = c_id
    and pro_type = 2
)
and exists (
    select
        *
    from property
    where pro_c_id = c_id
    and pro_type = 3
)
order by c_id;