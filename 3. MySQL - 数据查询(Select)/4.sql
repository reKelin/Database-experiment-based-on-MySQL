select
    c_name,
    c_phone,
    b_number
from client, bank_card
where c_id = b_c_id
and b_type = "储蓄卡"
order by c_id;