select
    c_name,
    c_id_card,
    sum(pro_income) as total_income
from  client
    inner join property 
    on pro_c_id = c_id
    and pro_status = "可用"
group by c_id
order by sum(pro_income) desc
limit 3;