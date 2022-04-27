select
    c_name,
    c_phone,
    c_mail
from client
where c_id_card like "4201%"
and not exists (
    select
        pro_type
    from property
    where pro_c_id = c_id
    and pro_type = 1
)
order by c_id;