select
    c_name, 
    c_id_card, 
    sum(pro_quantity * i_amount) as insurance_total_amount, 
    sum(pro_income) as insurance_total_revenue 
from v_insurance_detail 
group by c_id_card 
order by insurance_total_amount desc;