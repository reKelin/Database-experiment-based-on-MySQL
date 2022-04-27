select
    pro_income,
    count(*) as presence
from property
group by pro_income
having count(*) >= all(
    select
        count(*)
    from property 
    group by pro_income
); 
