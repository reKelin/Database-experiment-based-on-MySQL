select
    c_id,
    c_name,
    count(b_c_id) as number_of_cards
from client
    left join bank_card
    on c_id = b_c_id
where c_name like "黄%"
group by c_id
order by number_of_cards desc, c_id;