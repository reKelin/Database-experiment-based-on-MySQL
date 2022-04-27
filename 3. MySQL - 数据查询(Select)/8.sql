select
    c_name,
    c_id_card,
    c_phone
from client
where (c_id, "信用卡") in (
    select 
        b_c_id,
        b_type
    from bank_card
    group by b_c_id, b_type
    having count(*) > 1
);