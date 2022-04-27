-- (1) 基金总收益排名(名次不连续)

select
    pro_c_id,
    sum(pro_income) as total_revenue,
    rank() over(order by sum(pro_income) desc) as "rank"
from property 
where pro_type = 3
group by pro_c_id 
order by total_revenue desc, pro_c_id;

-- (2) 基金总收益排名(名次连续)

select
    pro_c_id,
    sum(pro_income) as total_revenue,
    dense_rank() over(order by sum(pro_income) desc) as "rank"
from property 
where pro_type = 3
group by pro_c_id 
order by total_revenue desc, pro_c_id;